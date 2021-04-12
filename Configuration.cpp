// 
// 
// 

#include "Configuration.h"

void Configuration::init()
{
//	Read();
}

//
// visualización en el display
//
void Configuration::Read() {
	/* read configuration struct from flash */
	byte* b = dueFlashStorage.readAddress(4); // byte array which is read from flash at adress 4
	memcpy(&configurationData, b, sizeof(configurationData)); // copy byte array to struct
	
	if (configurationData.initialized != 'Y')
	{
		configurationData.initialized = 'Y';
		configurationData.mode = 2;
		configurationData.hum_suelo_min = 25;
		configurationData.hum_suelo_max = 75;
		configurationData.hum_huerto_min = 50;
		configurationData.hum_huerto_max = 70;
		configurationData.temp_unit = 0;
		configurationData.temp_min = 5;
		configurationData.temp_max = 30;
		configurationData.interval = 24;
		configurationData.duration = 5;
		configurationData.timezone = 2;
		// write configuration struct to flash at adress 4
		byte b2[sizeof(ConfigurationData)]; // create byte array to store the struct
		memcpy(b2, &configurationData, sizeof(ConfigurationData)); // copy the struct to the byte array
		dueFlashStorage.write(4, b2, sizeof(ConfigurationData)); // write byte array to flash

	}
	
	mode = configurationData.mode;
	hum_suelo_min = configurationData.hum_suelo_min;
	hum_suelo_max = configurationData.hum_suelo_max;
	hum_huerto_min = configurationData.hum_huerto_min;
	hum_huerto_max = configurationData.hum_huerto_max;
	temp_unit = configurationData.temp_unit;
	temp_min = configurationData.temp_min;
	temp_max = configurationData.temp_max;
	interval = configurationData.interval;
	duration = configurationData.duration;
	timezone = configurationData.timezone;
}

/**
ESCRIBE LA MEMORIA EEPROM CON AL CONFIGURACION DE USUARIO
*/
void Configuration::Write()
{
	configurationData.initialized = 'Y';
	configurationData.mode = mode;
	configurationData.hum_suelo_min = hum_suelo_min;
	configurationData.hum_suelo_max = hum_suelo_max;
	configurationData.hum_huerto_min = hum_huerto_min;
	configurationData.hum_huerto_max = hum_huerto_max;
	configurationData.temp_unit = temp_unit;
	configurationData.temp_min = temp_min;
	configurationData.temp_max = temp_max;
	configurationData.interval = interval;
	configurationData.duration = duration;

	// write configuration struct to flash at adress 4
	byte b2[sizeof(ConfigurationData)]; // create byte array to store the struct
	memcpy(b2, &configurationData, sizeof(ConfigurationData)); // copy the struct to the byte array
	dueFlashStorage.write(4, b2, sizeof(ConfigurationData)); // write byte array to flash
}

void Configuration::Display(LiquidCrystal_I2C *pLCD, DateTime now) {

	// Mostrar el modo actual memory.d.mode

	pLCD->setCursor(0, 0);
	switch (mode)
	{
	case 0:
		pLCD->print("Fixed");
		pLCD->setCursor(5, 0);
		pLCD->print("ON");
		break;
	case 1:
		pLCD->print("Temp");
		pLCD->setCursor(5, 0);
		pLCD->print(duration);
		pLCD->print("m/");
		pLCD->print(interval);
		pLCD->print("h");
		break;
	case 2:
		pLCD->print("by sensors");
		break;
	}
}

void Configuration::Display(U8X8_SH1106_128X64_NONAME_HW_I2C *pLCD, DateTime now)
{

	// Mostrar el modo actual memory.d.mode

	pLCD->setCursor(0, 1);
	switch (mode)
	{
	case 0:
		pLCD->print("Fixed ");
		pLCD->setCursor(5, 1);
		pLCD->print("ON");
		break;
	case 1:
		pLCD->print("Temp");
		pLCD->setCursor(5, 1);
		pLCD->print(duration);
		pLCD->print("m/");
		pLCD->print(interval);
		pLCD->print("h");
		break;
	case 2:
		pLCD->print("by sensors");
		break;
	}
	pLCD->setCursor(11, 0);
	pLCD->print(now.hour());
	pLCD->print(":");
	pLCD->print(now.minute());

	//Serial.print(now.hour()); Serial.print(":"); Serial.println(now.minute());
}



//
//Datos Puerto Serie
//
void  Configuration::Monitor()
{
	String unidades;
	Serial.println("---------------CONFIGURATION----------------");
	Serial.print("modo:                  "); Serial.print(mode); Serial.print("-");
	switch (mode)
	{
	case 0:
		Serial.println("Fijo");
		break;
	case 1:
		Serial.println("Temporizado");
		break;
	case 2:
		Serial.println("Por sensores");
		break;
	}
	Serial.print("Humedad huerto maxima: "); Serial.print(hum_huerto_max); Serial.println("%");
	Serial.print("Humedad huerto minima: "); Serial.print(hum_huerto_min); Serial.println("%");
	Serial.print("Humedad suelo maxima:  "); Serial.print(hum_suelo_max); Serial.println("%");
	Serial.print("Humedad suelo minima:  "); Serial.print(hum_suelo_min); Serial.println("%");
	Serial.print("Unidades temperatura:  "); Serial.print(temp_unit);
	switch (temp_unit)
	{
	case 0:
		Serial.println("-Centigrados");
		unidades = "C";
		break;
	case 1:
		Serial.println("-farenheit");
		unidades = "F";
		break;
	}
	Serial.print("Temperatura minima:    "); Serial.print(temp_min); Serial.println(unidades);
	Serial.print("Temperatura maxima:    "); Serial.print(temp_max); Serial.println(unidades);
	Serial.print("Intervalo:             "); Serial.print(interval); Serial.println(" horas");
	Serial.print("Duracion:              "); Serial.print(duration); Serial.println(" minutos");
	Serial.print("Zona horaria:          GMT "); Serial.println(timezone);
	Serial.println("--------------------------------------------");
	Serial.println();

}
