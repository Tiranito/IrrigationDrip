// 
// 
// 

#include "Soil.h"


/*
* Inicializa los valores de la clase Soil
*	hum_huerto_min	Humedad mínima, por debajo de ella hay que regar 
*	hum_huerto_max	Humedad máxima hasta la que hay que regar
*	name_soil		Nombre de la zona de riego
*	lineLCD			Linea en la que se presentará en pantalla
*	pSuelo			Pin de lectura dispositivo de humedad
*	pOpen			Pin que abre la valvula de riego
*	pPower			Pin que alimenta el dispositivo lector de humedad
*
*
*/

void Soil::init(int humMin, int humMax, String name, byte line, byte pSuelo, byte pOpen, byte pPower) {

	// Iniciallizamos datos clase
	minHumSuelo = humMin;
	maxHumSuelo = humMax;
	nameSoil = name;
	LCDline = line;
	pinSuelo = pSuelo;
	pinOpen = pOpen;
	pinPower = pPower;
	riego = false;

	pinMode(pinSuelo, INPUT);	// pin lectura de humedad del suelo
	pinMode(pinPower, OUTPUT);	// pin alimentación del lector de humedad del suelo
	pinMode(pinOpen, OUTPUT);	// pin apertura de la valvua para riego

	digitalWrite(pinOpen, HIGH);	// Apagado 
}

//
// Adquisición humedad suelo
//
void Soil::Check() {
	
	// Encendemos el pin de alimentación previo a la lectura
	digitalWrite(pinPower, HIGH);
	delay(300);
	// Leemos sensor de humedad Suelo
	_humedadSuelo = getHumidityAvg(25, 10);
	humedadSuelo = map(_humedadSuelo, 0, 1023, 0, 100);
	// Encendemos el pin de alimentación previo a la lectura
	digitalWrite(pinPower, LOW);

	//Identificamos el estado del suelo
	if (humedadSuelo <  minHumSuelo)                                  strcpy(ESTADO_HUM, "DRY");	// Seco
	if (humedadSuelo >= minHumSuelo && humedadSuelo <= maxHumSuelo)   strcpy(ESTADO_HUM, "WET");	// Mojado
	if (humedadSuelo >  maxHumSuelo)                                  strcpy(ESTADO_HUM, "EXC");	// Exceso

}

//
// Mostrar datos en pantalla
//
void Soil::Display(LiquidCrystal_I2C *pLCD) {
	byte charDrip = 7;

	// Sensor humedad suelo
	pLCD->setCursor(0, LCDline);
	pLCD->print(nameSoil);
	pLCD->setCursor(7, LCDline);
	pLCD->print(humedadSuelo);
	pLCD->print("%");
	// Estado del riego / Depósito
	pLCD->setCursor(11, LCDline);
	if (riego)	pLCD->print("O");	// Riego pLCD->write(charDrip);
	else		pLCD->print("-");	// no Riego
}
void Soil::Display(U8X8_SH1106_128X64_NONAME_HW_I2C *pLCD) {
	byte charDrip = 7;
	
		// Sensor humedad suelo
	pLCD->setCursor(0, LCDline);
	pLCD->print(nameSoil);
	pLCD->setCursor(7, LCDline);
	pLCD->print(humedadSuelo);
	pLCD->print("%");
	// Estado del riego / Depósito
	pLCD->setCursor(11, LCDline);
	if (riego)	pLCD->print("O");	// Riego pLCD->write(charDrip);
	else		pLCD->print("-");	// no Riego
	pLCD->setCursor(13, LCDline);
	pLCD->print(ESTADO_HUM);
}
//
// Datos Puerto Serie
//
void Soil::Monitor() {
	Serial.println("----------------SOIL--------------------");
	Serial.print("-------  "); Serial.println(nameSoil);
	Serial.println("----------------------------------------");
	Serial.print("% Humedad: ");
	Serial.print(humedadSuelo);	Serial.println("%");
	Serial.print("lectura humedad: "); Serial.println(_humedadSuelo);
	Serial.print("Estado del cultivo: "); Serial.println(ESTADO_HUM);
	Serial.print("SituaciOn riego: ");
	if (riego)	Serial.println("Abierto");
	else		Serial.println("Cerrado");
	Serial.println("----------------------------------------");
	Serial.println();
}

//
// Abre el riego
//
void Soil::RiegoOn() {
	digitalWrite(pinOpen, LOW);	// Dispositivo funciona al revés
	riego = true;
}
//
// Abre el riego con una duracion
//
void Soil::RiegoOn(int duratiion) {
	digitalWrite(pinOpen, LOW);	// Dispositivo funciona al revés
	riego = true;
	// FAlta crear una interrupción para parar el riego en "duration" minutos
	// delay(duration * 60 * 1000);
	//
}

//
// Cierra el riego
//
void Soil::RiegoOff() {
	digitalWrite(pinOpen, HIGH);	// Dispositivo funciona al revés
	riego = false;
}

//
// Lee "count" veces la humedad del suelo, obteniendo la media
// previo desprecio de los valores extremos.
//
int Soil::getHumidityAvg(int wait, int count) {
	long min, max, avg, d;
	min = 999;
	max = 0;
	avg = d = 0;

	if (wait < 25)	wait = 25;
	if (count < 1) 	count = 1;

	for (int x = 0; x < count + 2; x++) {
		d = getHumidity();
		delay(wait);

		if (d < min) min = d;
		if (d > max) max = d;

		avg += d;
	}

	// substract highest and lowest value
	avg -= (max + min);
	// calculate average
	avg /= count;
	return avg;
}

int Soil::getHumidity() {

	return analogRead(pinSuelo);
}
