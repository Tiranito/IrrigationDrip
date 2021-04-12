// Configuration.h
#include <DueFlashStorage.h>
#include <U8x8lib.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

#ifndef _CONFIGURATION_h
#define _CONFIGURATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class Configuration {

public:
	char initialized;
	byte mode;
	byte hum_suelo_min;
	byte hum_suelo_max;
	byte hum_huerto_min;
	byte hum_huerto_max;
	byte temp_unit;
	byte temp_min;
	byte temp_max;
	byte interval;
	byte duration;
	byte timezone;
	DueFlashStorage dueFlashStorage;

	//New Configuration
	void init();	// Inicialización con valores por defecto
	void Read();	// Lee la última configuración guardada
	void Write();  	// Escribe la configuración en la EPRROM
	void Display(LiquidCrystal_I2C *, DateTime);		// Say in the screen (LiquidCrystal_I2C, temp_unit)
	void Display(U8X8_SH1106_128X64_NONAME_HW_I2C *, DateTime);	// Say in the screen (LiquidCrystal_I2C, temp_unit)
	void Monitor();	// Datos Puerto Serie

private:
	struct ConfigurationData {      // Estructura STRUCT con las variables que almacenaran los datos que se guardaran en la memoria EEPROM
		char initialized;
		byte mode;
		byte hum_suelo_min;
		byte hum_suelo_max;
		byte hum_huerto_min;
		byte hum_huerto_max;
		byte temp_unit;
		byte temp_min;
		byte temp_max;
		byte interval;
		byte duration;
		byte timezone;
	} configurationData;
	
	union MEMORY {       // Estructura UNION para facilitar la lectura y escritura en la EEPROM de la estructura STRUCT
		ConfigurationData d;
		byte b[sizeof(ConfigurationData)];
	}
	memory;
};


#endif
