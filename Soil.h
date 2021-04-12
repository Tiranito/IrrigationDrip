//
// Soil.h
//
#include <LiquidCrystal_I2C.h>
#include <U8x8lib.h>

#ifndef _SOIL_h
#define _SOIL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Soil
{
 protected:

 public:
	byte minHumSuelo = 0;
	byte maxHumSuelo = 0;
	String nameSoil;
	uint8_t LCDline;
	byte humedadSuelo = 0;
	char ESTADO_HUM[10];					//Situacion actual del Soil.{DRY,WET,EXC}
	boolean riego;


	//New Soil (hum_min, hum_max, name, line, pSuelo, pOpen, pPower)
	void init(int, int, String, byte, byte, byte, byte);
	void Check();							// Lectura sensores 
	void Display(LiquidCrystal_I2C *);		// Presenta datos en pantalla
	void Display(U8X8_SH1106_128X64_NONAME_HW_I2C *);		// Presenta datos en pantalla
	void Monitor();							// Vuelca datos en el monitor serie
	void RiegoOn();							// Activa riego
	void RiegoOn(int);						// Activa riego con duración
	void RiegoOff();						// Anula riego
private:
	byte pinSuelo = 0;
	byte pinOpen = 0;
	byte pinPower = 0;
	int _humedadSuelo;
	
	int getHumidityAvg(int, int);	// Obtiene la media de n lecturas
	int getHumidity();				// Obtiene la lectura de la humedad

};

#endif

