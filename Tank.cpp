// 
// 
// 

#include "Tank.h"

// Init values: Minimun level, ALT_POSTE, LARGO, ANCHO, ALTO, TRIG, ECHO, pinBonba, pinRed) 
void Tank::init(int mL, int ap, int l, int anch, int alt, int pTrig, int pEcho, byte pTank, byte pRed)
{
	pinEcho = pEcho;
	pinTrig = pTrig;
	pinTank = pTank;
	pinRed = pRed;
	sr04 = SR04(pEcho, pTrig);

	minLevel = mL;
	alt_poste = ap; // distancia desde el techo del depósito al sensor
	largo = l;
	ancho = anch;
	alto = alt;
	CAPACITY = ancho * largo * alto;

	pinMode(pTank, OUTPUT);
	pinMode(pRed, OUTPUT);

	// Inicialmente se apagan los dispositivos
	digitalWrite(pinTank, HIGH);	// Dispositivo funciona al revés
	digitalWrite(pinRed, HIGH);	// Dispositivo funciona al revés
	strcpy(SITUACION, "NONE");

}

//
//Adquisicion volumen actual
//
void Tank::Level()
{
	//Adqusición valor altura
	//distance = sr04.Distance();
	distance = sr04.DistanceAvg(25, 10);
	
	//Algoritmo calculo volumen 					
	level = largo * ancho * (alto - (distance - alt_poste));
	
	// Calculo del porcentaje de capacidad
	percentageCapacity = (level * 100) / CAPACITY;

	// Calculo del estado
	if (level <  minLevel) 		                    strcpy(ESTADO, "EMPTY");
	if (level >= minLevel && level <= CAPACITY) 	strcpy(ESTADO, "OK");
	if (level >  CAPACITY || level < 0)         	strcpy(ESTADO, "FAIL");
}

//
// visualización en el display
//
void Tank::Display(LiquidCrystal_I2C *pLCD) {
	byte charFull = 4;
	byte charHalf = 5;
	byte charFourth = 6;
	byte charEmpty = 7;
	
	pLCD->setCursor(13, 0);
	if (percentageCapacity < 25)									pLCD->write(charEmpty);
	if (percentageCapacity >= 25 && percentageCapacity <= 50)		pLCD->write(charFourth);
	if (percentageCapacity >= 50 && percentageCapacity <= 80)		pLCD->write(charHalf);
	if (percentageCapacity > 80)									pLCD->write(charFull);

	byte charTank = 8;
	byte charTap = 2;
	// Estado del riego / Depósito
	pLCD->setCursor(13, 1);
	if (strcmp(SITUACION, "TANK"))	pLCD->print("D"); // Depósito pLCD->write(charTank);
	else							pLCD->print("R"); // Red      pLCD->write(charTap);		
}
void Tank::Display(U8X8_SH1106_128X64_NONAME_HW_I2C *pLCD) {
	byte charFull = 4;
	byte charHalf = 5;
	byte charFourth = 6;
	byte charEmpty = 7;

	pLCD->setCursor(7, 5);
	pLCD->print("Tank ");

	if (percentageCapacity < 25)									pLCD->print("<25%");
	if (percentageCapacity >= 25 && percentageCapacity <= 50)		pLCD->print("<50%");
	if (percentageCapacity >= 50 && percentageCapacity <= 80)		pLCD->print("<80%");
	if (percentageCapacity > 80)									pLCD->print("100%");

}
//
//Datos Puerto Serie
//
void  Tank::Monitor()
{
	Serial.println("---------------TANK---------------------");
	Serial.print("Medidas del tanque: ");
	Serial.print(ancho);
	Serial.print(" x ");
	Serial.print(largo);
	Serial.print(" x ");
	Serial.print(alto);
	Serial.print(" = ");
	Serial.print(CAPACITY);
	Serial.println(" cm3");

	Serial.print("Distance: ");
	Serial.print(distance);
	Serial.println(" cm");

	Serial.print("% Capacidad: ");
	Serial.print(percentageCapacity);
	Serial.println("%");

	Serial.print("Nivel minimo Tanque: ");
	Serial.println(minLevel);

	Serial.print("Nivel actual del Tanque: ");
	Serial.print(level);
	Serial.println(" cm3");

	Serial.print("Estado: ");
	Serial.println(ESTADO);

	Serial.print("Situacion: ");
	Serial.println(SITUACION);

	Serial.print("pinTank: ");
	Serial.println(digitalRead(pinTank));
	Serial.print("pinRed: ");
	Serial.println(digitalRead(pinRed));

	Serial.println("----------------------------------------");
	Serial.println();

}

//
// Abrir bomba
//
void  Tank::On()
{
	if (strcmp(ESTADO, "OK") == 0) {
		digitalWrite(pinTank, LOW);	// Dispositivo funciona al revés
		digitalWrite(pinRed, HIGH);	// Dispositivo funciona al revés
		strcpy(SITUACION, "TANK");

	}
	else
	{
		digitalWrite(pinTank, HIGH);	// Dispositivo funciona al revés
		digitalWrite(pinRed, LOW);	// Dispositivo funciona al revés
		strcpy(SITUACION, "NET");
	}
}

//
// Cerrar bomba
//
void  Tank::Off()
{
	digitalWrite(pinTank, HIGH);	// Dispositivo funciona al revés
	digitalWrite(pinRed, HIGH);	// Dispositivo funciona al revés
	strcpy(SITUACION, "NONE");
}


