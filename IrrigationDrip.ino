/*
Gestor de riego por goteo...


*/

#define SH1106
#define DEBUG

#include "Climate.h"
#include "Soil.h"
#include "Configuration.h"
#include "Tank.h"
#include "Logger.h"

#include <Wire.h>
#include <DueTimer.h>

#include <RTClib.h>

#ifdef SH1106
#include <U8g2lib.h>
#else
#include <LiquidCrystal_I2C.h>
#endif // SH1106

/*
	DECLARACIÓN DE OBJETOS
*/
Configuration config;
Tank tanque;
Soil huerto;
Soil campo;
ClimateClass sky;
Logger logger;
RTC_DS3231 rtc;

#ifdef SH1106
U8X8_SH1106_128X64_NONAME_HW_I2C lcd(/* reset=*/ U8X8_PIN_NONE);
const byte rowsLCD = 8;                 // Filas del LCD
const byte columnsLCD = 16;             // Columnas del LCD
#else
LiquidCrystal_I2C lcd(0x3F, 20, 4);
const byte rowsLCD = 4;                 // Filas del LCD
const byte columnsLCD = 20;             // Columnas del LCD
#endif // SH1106

/*
	DECLARACION DE PINES
*/
const byte pOpnTank = 14; // Relay puesta en marcha bomba del depósito
const byte pOpnHuerto = 15;// Relay apertura linea riego del huerto
const byte pOpnSuelo = 16; // Relay apertura linea riego del suelo
const byte pOpnRed = 17;  // Relay apertura agua de la red

const byte pENCO_SW = 3;  // Pin encoder SW
const byte pENCO_DT = 2;  // Pin encoder DT
const byte pENCO_CLK = 4; // Pin encoder CLK

const byte pEcho = 8;     // Sensor nivel depósito Echo
const byte pTrig = 9;     // Sensor nivel depósito trigger

const byte pRainPower = 5; // VCC sensor lluvia -- no implementado de momento
const byte pRain = A2;     // Sensor lluvia

const byte pLight = A3;    // Sensor luminosidad

const byte pCampo = A0;    // Sensor humedad suelo analógico 
const byte pPowerSuelo = 7;// VCC sensor Suelo

const byte pHuerto = A1;    // Sensor humedad huerto analógico
const byte pPowerHuerto = 6;// VCC sensor Huerto

const byte pLOG = 10;       // Tarjeta micro SD

#define COUNT(x) sizeof(x)/sizeof(*x)	// Macro para contar el numero de elementos de un array


enum Button { Unknown, Ok, Left, Right };
int btnPressed;								// Enumerador con los diferentes botones disponibles
enum Screen { Menu1, Menu2, Flag, Number }; // Enumerador con los distintos tipos de submenus disponibles

const char *txMENU[] = {       // Los textos del menu principal, la longitud maxima = columnsLCD-1, rellenar caracteres sobrantes con espacios.
	"Modo           ",
	"Hum. suelo min ",
	"Hum. suelo max ",
	"Hum. huerto min",
	"Hum. huerto max",
	"Unid. temp.    ",
	"Temperatura min",
	"Temperatura max",
	"Intervalo      ",
	"Duracion       ",
	"Zona horaria   ",
	"Guardar y salir",
	"Salir          "
};
const byte iMENU = COUNT(txMENU);			// Numero de items/opciones del menu principal

enum eSMENU1 { Fixed, Periodic, Irrigation };	// Enumerador de las opciones disponibles del submenu 1 (tienen que seguir el mismo orden que los textos)
const char *txSMENU1[] = {					// Textos del submenu 1, longitud maxima = columnsLCD-2, rellenar caracteres sobrantes con espacios
	"     Fijo     ",
	" Temporizador ",
	"   Sensores   "
};

enum eSMENU2 { GradeC, GradeF };			// Enumerador de las opciones disponibles del submenu 2 (tienen que seguir el mismo orden que los textos)
const char *txSMENU2[] = {					// Textos del submenu 1, longitud maxima = columnsLCD-2, rellenar caracteres sobrantes con espacios
	"   Grados C   ",
	"   Grados F   "
};


void setup()
{	
	// Serial
	Serial.begin(115200);
	
	// Configuracion
	config.Read();
#ifdef DEBUG
	config.Monitor();
#endif

	// Logger
	logger.init(pLOG);
	logger.WriteConfig(config.mode, config.hum_suelo_min, config.hum_suelo_max, config.hum_huerto_min, config.hum_huerto_max, 
		config.temp_unit, config.temp_min, config.temp_max, config.interval, config.duration);
	logger.WriteHeader();

	// Campo  
	huerto.init(config.hum_huerto_min, config.hum_huerto_max, "Huerto", 2, pHuerto, pOpnHuerto, pPowerHuerto);
	campo.init(config.hum_suelo_min, config.hum_suelo_max, "Campo", 3, pCampo, pOpnSuelo, pPowerSuelo);

	// Clima
	sky.init(config.temp_min, config.temp_max, pLight, pRain, 39.430358, -0.563517, config.timezone);
	
	// Tanque  min_Level,  aalto_poste,  largo,  anch,  alt,  pTrig,  pEcho, pTank, pRed
	tanque.init(100, 0, 30, 30, 30, pTrig, pEcho, pOpnTank, pOpnRed);

	// Rotary encoder
	pinMode(pENCO_SW, INPUT_PULLUP);
	pinMode(pENCO_DT, INPUT_PULLUP);
	pinMode(pENCO_CLK, INPUT_PULLUP);

	// Inicia el LCD
#ifdef SH1106
	lcd.begin();
	lcd.setFont(u8x8_font_chroma48medium8_r);
#else
	#include "SymbolsLCD.h"
	lcd.init();
	lcd.backlight();
	lcd.createChar(iARROW, bARROW);
	lcd.createChar(iDEGREE, bDEGREE);
	lcd.createChar(iTAP, bTAP);
	lcd.createChar(iDRIP, bDRIP);
	lcd.createChar(iFULL, bFULL);
	lcd.createChar(iHALF, bHALF);
	lcd.createChar(iFOURTH, bFOURTH);
	lcd.createChar(iEMPTY, bEMPTY);
#endif // SH1106

	// Reloj
	if (!rtc.begin()) {
		Serial.println(F("Couldn't find RTC"));
	}
	// Si se ha perdido la corriente, fijar fecha y hora
	if (rtc.lostPower()) {
		// Fijar a fecha y hora de compilacion
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	}

	// Mostrar pantalla con mensaje bienvenida
	Wellcome();
}

void loop()
{
	const long interval = 5000;
	const long saveInterval = 4000;
	static unsigned long tSavePower = 0;
	bool enableSavePower = false;
	
	static unsigned long tNow = 0;
	static unsigned long tPrevious = 0;

	tNow = millis();
	btnPressed = readButtons();
	if (btnPressed == Button::Ok) {
		openMenu();
	}

	// Pinta la pantalla principal cada "interval" segundos
	if (tNow - tPrevious >= interval)
	{
		tPrevious = tNow;

		// se leen los valores de los sensores
		DateTime now = rtc.now();
#ifdef DEBUG
		Serial.print("Fecha:"); Serial.print(now.day()); Serial.print("-"); Serial.print(now.month());; Serial.print("-"); Serial.println(now.year());
		Serial.print("Hora:"); Serial.print(now.hour()); Serial.print(":"); Serial.println(now.minute());
#endif // DEBUG

		sky.Check(now);
		huerto.Check();
		campo.Check();
		tanque.Level();

		// se muestran los valores en pantalla
		config.Display(&lcd, now);
		sky.Display(&lcd, config.temp_unit);
		huerto.Display(&lcd);
		campo.Display(&lcd);
		tanque.Display(&lcd);

		// monitorizamos
#ifdef DEBUG
		sky.Monitor();
		huerto.Monitor();
		campo.Monitor();
		tanque.Monitor();
#endif // DEBUG

		// Grabamos en el logger
		logger.Write(now, config.mode, campo.riego, huerto.riego, 
			campo.humedadSuelo, huerto.humedadSuelo, 
			sky.temperature, sky.humidity, sky.pressure, sky.rain, tanque.level);

		// Comprobamos el modo de funcionamiento
		switch (config.mode)
		{
		case eSMENU1::Fixed:      modeFixed(); break;
		case eSMENU1::Periodic:   modeTimer(); break;
		case eSMENU1::Irrigation: modeDripIrrigation(); break;
		}
	}
}
