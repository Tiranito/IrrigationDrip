// 
// Gestiona todos los sensores relacionados con el clima
// Temperatura, humedad, presion, lluvia, luz
// 

#include "Climate.h"

void ClimateClass::init(byte tempMin, byte tempMax, byte pLight, byte pRain, float latitude, float longitud, int timezone)
{
	pinRain  = pRain;
	pinLight = pLight;
	_tempMin = tempMin;
	_tempMax = tempMax;
	_latitude = latitude;
	_longitude = longitud;

	pinMode(pinRain, INPUT);
	pinMode(pinLight, INPUT);

	mySunrise.Init(latitude, longitud, timezone);

	if (climateSensor.beginI2C() == false) {		//Begin communication over I2C
		Serial.println("ERROR: The chip BME280 did not respond. Please check wiring.");
	}
	if (!rtc.begin()) {
		Serial.println("ERROR: Couldn't find RTC");
	}
}

//
// Lee los sensores climaticos: Humedad, temperatura, presion, lluvia, luminosidad
//
void ClimateClass::Check(DateTime now) {

	size_t elementIni, elementEnd;
	int i;
	int luminosidad;
	
	// RAIN DROPS
	_rain = analogRead(pinRain);
	if (_rain > 50) rain = HIGH;
	else			rain = LOW;
	//Serial.print("Sensor rain "); Serial.print(pinRain); Serial.print(": "); Serial.print(rain); Serial.print(" - "); Serial.println(_rain);

	// LUMINOSIDAD
//	light = getLuminosity();
	light = getLuminosityAvg(25, 10);
	calculateSunset(now);		//solo deberia ejecutarse al cambio de dia

	// Si ya tenemos 6 elementos borramos el primero, y siguimos añadiendo
	if (_list.Count() > 5) _list.RemoveFirst();
	_list.Add(light);

  	// El estado se decide por:
  	// Si llueve 							-> RAIN
  	// Si luminosidad es minima				-> NIGHT
	// Si la hora está entre el amanecer	-> DAWN
	// Si la hora está entre el anochecer	-> DUSK
  	// en funcion de la liminosidad DAY or CLODY
  	if (rain)							strcpy(ESTADO, "RAIN ");	// LLuvia
	else if (compareTimes(0, 0, _hourSunriseFrom, _minutesSunriseFrom, now) || compareTimes(_hourSunsetTo, _minutesSunsetTo, 23, 59, now))	strcpy(ESTADO, "NIGHT");	// Noche
	else if (compareTimes(_hourSunriseFrom, _minutesSunriseFrom, _hourSunriseTo, _minutesSunriseTo, now))	strcpy(ESTADO, "DAWN ");	// Amanecer
  	else if (compareTimes(_hourSunsetFrom, _minutesSunsetFrom, _hourSunsetTo, _minutesSunsetTo, now))		strcpy(ESTADO, "DUSK ");	// Anochecer
  	else if (light < 400)				strcpy(ESTADO, "DAY  ");	// durante el dia y está soleado
	else if (light > 400)				strcpy(ESTADO, "CLODY");	// Nublado


	// HUMEDAD
	humidity = climateSensor.readFloatHumidity();

	// ALTITUDE
	altitude = climateSensor.readFloatAltitudeMeters();
	//altitude = climateSensor.readFloatAltitudeFeet();

	// TEMPERATURA
	temperature = climateSensor.readTempC();
	//temperature = climateSensor.readTempF();	
	
	if (temperature <= _tempMin)	    strcpy(ESTADO_TEMP, "COLD");	// Frio
	else if (temperature >= _tempMax)	strcpy(ESTADO_TEMP, "HOT ");	// Calor
	else								strcpy(ESTADO_TEMP, "GOOD");	// Normal

	// PRESION
	pressure = climateSensor.readFloatPressure() / 100;

}

//
// Presenta datos en LCD
//
void ClimateClass::Display(LiquidCrystal_I2C *pLCD, byte unidadesTemp) {
	enum unidTemp { GradeC, GradeF };

	// Temperatura
	//pLCD->setCursor(14, 0);
	//pLCD->print("T:");
	pLCD->setCursor(16, 0);
	switch (unidadesTemp)
	{
	case unidTemp::GradeC:
		pLCD->print((byte)temperature);
		pLCD->write(1);
		break;
	case unidTemp::GradeF:
		pLCD->print((byte)(1.8 * temperature + 32));
		pLCD->print(" F");
		break;
	}
	// Humedad de ambiente
	//pLCD->setCursor(14, 1);
	//pLCD->print("H:");
	pLCD->setCursor(16, 1);
	pLCD->print((int)humidity);
	pLCD->print("%");

	// Presi�n atmosferica
	pLCD->setCursor(14, 2);
	pLCD->print((int)pressure);

	// Luminosity status (RAIN, NIGHT, DAY, DAWN, DUSK)
	pLCD->setCursor(14, 3);
	pLCD->print(ESTADO);
}
void ClimateClass::Display(U8X8_SH1106_128X64_NONAME_HW_I2C *pLCD, byte unidadesTemp) {
	enum unidTemp { GradeC, GradeF };
	uint8_t bDEGREE[8] = {
		0b00001100, 0b00010010, 0b00010010, 0b00001100,
		0b00000000, 0b00000000, 0b00000000, 0b00000000
	};

	// Temperatura
	pLCD->setCursor(1, 5);
	switch (unidadesTemp)
	{
	case unidTemp::GradeC:
		pLCD->print((byte)temperature);
		pLCD->print("C");
		//pLCD->drawTile(1, 7, 1, bDEGREE);
		break;
	case unidTemp::GradeF:
		pLCD->print((byte)(1.8 * temperature + 32));
		pLCD->print(" F");
		break;
	}
	// Humedad de ambiente
	pLCD->setCursor(1, 6);
	pLCD->print((int)humidity);
	pLCD->print("%");

	// Presi�n atmosferica
	pLCD->setCursor(0, 7);
	pLCD->print((int)pressure);

	// Luminosity status (RAIN, NIGHT, DAY, DAWN, DUSK)
	pLCD->setCursor(11, 7);
	pLCD->print(ESTADO);
}


//
// Datos Puerto Serie
//
void ClimateClass::Monitor() {
	Serial.println("------------CLIMATE---------------------");
	Serial.print("Estado:      ");
	Serial.println(ESTADO);
	Serial.print("Estado Temp:      ");
	Serial.println(ESTADO_TEMP);
	Serial.print("Temperatura: ");
	Serial.print(temperature);
	Serial.println("C");
	Serial.print("Humedad:     ");
	Serial.print(humidity);
	Serial.println("%");
	Serial.print("Presion ATM: ");
	Serial.println(pressure);
	Serial.print("Llueve?      ");
	Serial.println(rain);
	Serial.print("Luminosidad: ");
	Serial.println(light);
	Serial.print("Altitud:     ");
	Serial.print(altitude); Serial.println(" m");
	Serial.print("Amanece desde :  ");
	Serial.print(_hourSunriseFrom); Serial.print(":"); Serial.print(_minutesSunriseFrom); Serial.print(" hasta:  ");	Serial.print(_hourSunriseTo); Serial.print(":"); Serial.println(_minutesSunriseTo);
	Serial.print("Anochece desde: "); 
	Serial.print(_hourSunsetFrom); Serial.print(":"); Serial.print(_minutesSunsetFrom); Serial.print(" hasta: ");	Serial.print(_hourSunsetTo); Serial.print(":"); Serial.println(_minutesSunsetTo);
	Serial.println("----------------------------------------");
	Serial.println("");
}

int ClimateClass::getLuminosity() {
	return analogRead(pinLight);
}

int ClimateClass::getLuminosityAvg(int wait, int count) {
	long min, max, avg, d;
	min = 999;
	max = 0;
	avg = d = 0;

	if (wait < 25)	wait = 25;
	if (count < 1) 	count = 1;

	for (int x = 0; x < count + 2; x++) {
		d = getLuminosity();
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
/*
*	Invoca a un servicio web rest para saber la predicción del tiempo
*	https://opendata.aemet.es/opendata/api/prediccion/especifica/municipio/diaria/46110
*
*/
bool ClimateClass::getWeatherForecast(long year, int month, int day) {
	return false;
}


float ClimateClass::getHumidity() {
	return humidity = climateSensor.readFloatHumidity();
}

float ClimateClass::getTemperature() {
	return temperature = climateSensor.readTempC();
	//temperature = climateSensor.readTempF();	
}

float ClimateClass::getPresure() {
	return pressure = climateSensor.readFloatPressure() / 100;
}

// calculates when dawn and dusk
void ClimateClass::calculateSunset(DateTime now) {
	byte h, m;
	int t;	 // t= minutes past midnight of sunrise (6 am would be 360)	

	mySunrise.Civil(); //Actual, Civil, Nautical, Astronomical
	t = mySunrise.Rise(now.month(), now.day()); // (month,day) - january=1
	if (t >= 0) {
		_hourSunriseFrom = mySunrise.Hour();
		_minutesSunriseFrom = mySunrise.Minute();
	}
	else { //There are either penguins or polar bears around here! 
		_hourSunriseFrom = 0;
		_minutesSunriseFrom = 0;
	}
	t = mySunrise.Set(now.month(), now.day()); // (month,day) - january=1
	if (t >= 0) {
		_hourSunsetTo = mySunrise.Hour();
		_minutesSunsetTo = mySunrise.Minute();
	}
	else { // 
		_hourSunsetTo = 0;
		_minutesSunsetTo = 0;
	}


	mySunrise.Actual(); //Actual, Civil, Nautical, Astronomical
	t = mySunrise.Rise(now.month(), now.day()); // (month,day) - january=1
	if (t >= 0) {
		_hourSunriseTo = mySunrise.Hour();
		_minutesSunriseTo = mySunrise.Minute();
	}
	else { // 
		Serial.println("There are either penguins or polar bears around here!");
	}
	t = mySunrise.Set(now.month(), now.day()); // (month,day) - january=1
	if (t >= 0) {
		_hourSunsetFrom = mySunrise.Hour();
		_minutesSunsetFrom = mySunrise.Minute();
	}
	else { // 
		Serial.println("There are either penguins or polar bears around here!");
	}
}


bool ClimateClass::compareTimes(unsigned char hourFrom, unsigned char minutesFrom, unsigned char hourTo, unsigned char minutesTo, DateTime now) {
	bool result = false;

	// Comparamos que la fecha actual esté dentro de el rango From and To
	if (now.hour() >= hourFrom && now.hour() <= hourTo) {
		//Se comprueba que en el caso de que sea la hora de inicio los minutos sean mayores o iguales
		if (now.hour() == hourFrom) {
			if (now.minute() >= minutesFrom) {
				//Se comprueba que en el caso de que sea la hora de fin los minutos sean menores o iguales
				if (now.hour() == hourTo) {
					if (now.minute() <= minutesTo) {
						result = true;
					}
					else {
						result = false;
					}
				}
				result = true;
			}
			else {
				result = false;
			}
		}
		else {
			result = true;
		}
	}
	else {
		result = false;
	}

	// Si el resultado sigue siendo correcto
	if (result) {
		// Se comprueba que en el caso de que sea la hora de fin, los minutos sean menores o iguales
		if (now.hour() == hourTo) {
			if (now.minute() <= minutesTo) {
				result = true;
			}
			else {
				result = false; 
			}
		}
		else {
			result = true; // no coincide con la hora final, pero eso no es erroneo
		}
	}

	return result;
}


ClimateClass Climate;

