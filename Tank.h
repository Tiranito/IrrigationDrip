// Tank.h

#ifndef _TANK_h
#define _TANK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include "SR04.h"
#include <LiquidCrystal_I2C.h>
#include <U8x8lib.h>

/**
* Constructor
* Manage a Tank, for open or not, 
* if level is higher than the minimun level
* 
* We need Ultrasonic sensor SR04, four connections pins
* VCC, ECHO, TRIGGER, GND
* 
* \param minimun level to open the tank
* \param distance betwwen tapa and the device
* \param long of tank
* \param width of tank
* \param hight of tank
* \param echoPin digital INPUT-Pin for measuring distance
* \param triggerPin if 10us high a trigger signal is generated from	SR04
*
* \return void
*/

class Tank
{
protected:
	SR04 sr04 = SR04(5, 6);

public:
	int minLevel;						// Nivel mínimo del tanque
	int alt_poste, largo, ancho, alto;	// Medidas del tanque
	int level;							// Volumen de agua
	int CAPACITY;						// Capacidad máxima del Tank
	char ESTADO[10];					// Situación actual del Tank.{EMPTY,OK,FAIL}
	char SITUACION[10];					// Situación actual del Tank.{NOTE, TANK, NET}
	int percentageCapacity; 			// Porcentaje capacidad del tanque
	long distance;

	void init(int, int, int, int, int, int, int, byte, byte);	//New Tank (LEVEL_min, ALT_POSTE, LARGO, ANCHO, ALTO, TRIG, ECHO, pinBonba, pinRed) 
	void Level();						// Adquisicion volumen actual
	void Display(LiquidCrystal_I2C *);	// Presenta datos en LCD
	void Display(U8X8_SH1106_128X64_NONAME_HW_I2C *);	// Presenta datos en LCD
	void Monitor();						// Datos Puerto Serie};
	void On();							// Enciende la bonba si hay subiciente nivel, o abre la red
	void Off();							// Apaga bomba y red

private:
	byte pinTrig;	// pin trigger del lector de ultrasonidos
	byte pinEcho;	// pin echo del lector de ultrasonidos
	byte pinTank;	// pin apertura del deposito
	byte pinRed;	// pin apertura de la red

};	
#endif
									