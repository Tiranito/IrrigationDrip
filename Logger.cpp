// 
// Clase Log Graba info en la SD
// 

#include "Logger.h"


//
// Constructor
//
void Logger::init(byte pLOG)
{
	if (!SD.begin(pLOG)) {
		initLog = false;
		//Serial.println("Error: no se ppuede inicializar el log");
	}
	else  initLog = true;

	
	//RestClient client = RestClient(IP, PORT);
	// Connect via DHCP
	//client.dhcp();
}


//
// Escritura de la cabecera del fichero excel
//
void Logger::WriteHeader() {
	// Abrir archivo y escribir valor
	logFile = SD.open("datalog.csv", FILE_WRITE);

	if (logFile) {
		logFile.print("Hora");
		logFile.print(";");
		logFile.print("Modo");
		logFile.print(";");
		logFile.print("Suelo Activo");
		logFile.print(";");
		logFile.print("Huerto Activo");
		logFile.print(";");
		logFile.print("Humedad Suelo");
		logFile.print(";");
		logFile.print("Humedad Huerto");
		logFile.print(";");
		logFile.print("Temperatura");
		logFile.print(";");
		logFile.print("Humedad");
		logFile.print(";");
		logFile.print("Presion");
		logFile.print(";");
		logFile.print("LLuvia");
		logFile.print(";");
		logFile.print("Nivel Deposito");
		logFile.println(";");

		logFile.close();
	}
}

//
// Escritura del registro detalle del fichero excel
//
void Logger::Write(DateTime now, byte mode, bool openSuelo, bool openHuerto, byte humSuelo, byte humHuerto, byte temp, byte humid, int presure, bool rain, int levelDepo) {
	
	// Abrir archivo y escribir valor
	logFile = SD.open("datalog.csv", FILE_WRITE);
	
	if (logFile) {
		logFile.print(now.year());logFile.print("-");logFile.print(now.month());logFile.print("-");logFile.print(now.day());logFile.print(" "); // Fecha
		logFile.print(now.hour());logFile.print(":");logFile.print(now.minute()); // Hora
		logFile.print(";");
		logFile.print(mode);            // Modo
		logFile.print(";");
		logFile.print(openSuelo);       // Indicador apertura Suelo
		logFile.print(";");
		logFile.print(openHuerto);      // Indicador apertura Huerto
		logFile.print(";");
		logFile.print(humSuelo);        // Humedad Suelo
		logFile.print(";");
		logFile.print(humHuerto);       // Humedad Huerto
		logFile.print(";");
		logFile.print(temp);            // Temperatura
		logFile.print(";");
		logFile.print(humid);           // Humedad
		logFile.print(";");
		logFile.print(presure);         // Presión
		logFile.print(";");
		logFile.print(rain);            // Luvia
		logFile.print(";");
		logFile.print(levelDepo);       // Nivel del depósito
		logFile.println(";");

		logFile.close();
	}
}

//
// Escritura de los datos de configuración
//
void Logger::WriteConfig(byte mode, byte humSueloMin, byte humSueloMax, byte humHuertoMin, byte humHuertoMax, byte tempUnit, byte tempMin, byte tempMax, byte interval, byte duration) {

	// Abrir archivo y escribir valor
	logFile = SD.open("datalog.csv", FILE_WRITE);

	if (logFile) {
		// cabecera de configuración
		logFile.print("Modo");
		logFile.print(";");
		logFile.print("Humedad Mínima Suelo");
		logFile.print(";");
		logFile.print("Humedad Máxima Suelo");
		logFile.print(";");
		logFile.print("Humedad Mínima Huerto");
		logFile.print(";");
		logFile.print("Humedad Macima Huerto");
		logFile.print(";");
		logFile.print("Unidad temperatura");
		logFile.print(";");
		logFile.print("temperatura minima");
		logFile.print(";");
		logFile.print("Temperatura máxima");
		logFile.print(";");
		logFile.print("Intervalo");
		logFile.print(";");
		logFile.print("Duración");
		logFile.println(";");

		// Línea de configuración
		logFile.print(mode);          // Modo
		logFile.print(";");
		logFile.print(humSueloMin);   // Humedad Suelo mínima
		logFile.print(";");
		logFile.print(humSueloMax);   // Humedad Suelo máxima
		logFile.print(";");
		logFile.print(humHuertoMin);  // Humedad Huerto minima
		logFile.print(";");
		logFile.print(humHuertoMax);  // Humedad Huerto máxima
		logFile.print(";");
		logFile.print(tempUnit);      // Unidades temperatura
		logFile.print(";");
		logFile.print(tempMin);       // Temperatura minima de riego
		logFile.print(";");
		logFile.print(tempMax);       // Temperatura máxima de riego
		logFile.print(";");
		logFile.print(interval);      // Temperatura máxima de riego
		logFile.print(";");
		logFile.print(duration);      // Temperatura máxima de riego
		logFile.println(";");

		logFile.close();
	}

}

void getDateTime() {
/*	response = "";
	client.setHeader("Authorization: Basic cmljdmVhbDoxMjM0==");
	client.setHeader("Content-Type: application/json");
	StaticJsonBuffer<200> jsonBuffer;
	char json[256];
	JsonObject& root = jsonBuffer.createObject();
	root["temperature"] = t;
	root.printTo(json, sizeof(json));

	int statusCode = client.post("/api/v1.0/temperature", json, &response);
	Serial.print("Status code from server: ");
	Serial.println(statusCode);
	Serial.print("Response body from server: ");
	Serial.println(response);
*/
}

