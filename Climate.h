// Climate.h
#include <ListLib.h>
#include "Wire.h"
#include "SparkFunBME280.h"
#include <LiquidCrystal_I2C.h>
#include <U8x8lib.h>
#include <Sunrise.h>
#include <RTClib.h>

#ifndef _CLIMATE_h
#define _CLIMATE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class ClimateClass
{
 
 public:
	Sunrise mySunrise;		// Calcular sunrise y sunset
	BME280 climateSensor;	// Global sensor object
	RTC_DS3231 rtc;			// real time clock

	float temperature = 0;	// Temperatura ambiente
	float humidity = 0;		// Humedad ambiente
	float pressure = 0;		// Presión ambiental
	float altitude = 0;		// Altitude
	int  light = 0;			// Nivel lumínico
	bool rain;				// Indicador llúvia
	char ESTADO[10];		// Situation of climate  
	char ESTADO_TEMP[5];	// Situacion actual ambiente.{COLD,HOT,GOOD}
	char sunsetFrom, sunsetTo, sunriseFrom, sunriseTo;
	unsigned char _hourSunsetFrom, _hourSunsetTo;
	unsigned char _minutesSunsetFrom, _minutesSunsetTo;
	unsigned char _hourSunriseFrom, _hourSunriseTo;
	unsigned char _minutesSunriseFrom, _minutesSunriseTo;



	void init(byte, byte, byte, byte, float, float, int);	// Initialize the class (temp_min, temp_max, pinLight, pinRain, lati, long, timeset)
	void Check(DateTime);							// Read all sensors
	void Display(LiquidCrystal_I2C *, byte);		// Say in the screen (LiquidCrystal_I2C, temp_unit)
	void Display(U8X8_SH1106_128X64_NONAME_HW_I2C *, byte);	// Say in the screen (LiquidCrystal_I2C, temp_unit)
	void Monitor();									// Output to Serial

	int getLuminosity();			// Get the luminosity
	int getLuminosityAvg(int, int);	// Get the average of a set of values of the luminosity
	float getHumidity();
	float getTemperature();
	float getPresure();
	void calculateSunset(DateTime);		// calculates when dawn and dusk

private:
	int _rain;
	byte pinRain;
	byte pinLight;
	byte _tempMin;
	byte _tempMax;

	List<int> _list;
	float _latitude;
	float _longitude;


	bool getWeatherForecast(long, int, int); // devuelve la predicción del tiempo para saber si llueve o no
	bool compareTimes(unsigned char, unsigned char, unsigned char, unsigned char, DateTime);

};

extern ClimateClass Climate;

#endif

