// Log.h

#ifndef _LOG_h
#define _LOG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SD.h"
#include "RTClib.h"

class Logger
{
 protected:


 public:
	bool initLog = false;
	File logFile;						// Fichero de log
	RTC_DS3231 rtc;

	void init(byte);
	void Write(DateTime, byte, bool, bool, byte, byte, byte, byte, int, bool, int);
	void WriteHeader();
	void WriteConfig(byte, byte, byte, byte, byte, byte, byte, byte, byte, byte);
	 
private:
	void getDateTime();
};


#endif

