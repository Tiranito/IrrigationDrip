// SymbolsLCD.h

#ifndef _SYMBOLSLCD_h
#define _SYMBOLSLCD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


uint8_t iARROW = 0;					// ID icono flecha
uint8_t bARROW[] = {				// Bits icono flecha
	B00000, B00100, B00110, B11111,
	B00110, B00100, B00000, B00000
};
uint8_t iDEGREE = 1;				// ID icono grado
uint8_t bDEGREE[8] = {
	0b00001100, 0b00010010, 0b00010010, 0b00001100,
	0b00000000, 0b00000000, 0b00000000, 0b00000000
};
uint8_t iTAP = 2;					// ID icono TAP
uint8_t bTAP[8] = {
	0b00011100, 0b00001001, 0b00111111, 0b10000000,
	0b00110000, 0b00110000, 0b00110000, 0b00110000
};
uint8_t iDRIP = 3;					// ID icono DRIP
uint8_t bDRIP[8] = {
	0b00010000, 0b00010000, 0b00011000, 0b00111000,
	0b01111100, 0b01111100, 0b00111000, 0b00000000
};
uint8_t iFULL = 4;					// ID icono tank full
uint8_t bFULL[8] = {
	0b01111100, 0b01111100, 0b01111100, 0b01111100,
	0b01111100, 0b01111100, 0b01111100, 0b01111100
};
uint8_t iHALF = 5;					// ID icono half
uint8_t bHALF[8] = {
	0b01000100, 0b01000100, 0b01000100, 0b01000100,
	0b01111100, 0b01111100, 0b01111100, 0b01111100
};
uint8_t iFOURTH = 6;					// ID icono fourth
uint8_t bFOURTH[8] = {
	0b01000100, 0b01000100, 0b01000100, 0b01000100,
	0b01000100, 0b01000100, 0b01111100, 0b01111100
};
uint8_t iEMPTY = 7;					// ID icono empty
uint8_t bEMPTY[8] = {
	0b01000100, 0b01000100, 0b01000100, 0b01000100,
	0b01000100, 0b01000100, 0b01000100, 0b01111100
};

#endif

