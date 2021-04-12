//
// FUNCION BIENVENIDA
//
void Wellcome() {
	// Imprime la informacion del proyecto:
	lcd.setCursor(0, 0); lcd.print("  GESTOR RIEGO  ");
	lcd.setCursor(0, 1); lcd.print("    Gabriel     ");
	lcd.setCursor(0, 2); lcd.print("Ver 1.0  may-18 ");
	lcd.setCursor(0, 3);
	for (int i = 0; i < columnsLCD; i++)
	{
		lcd.print(".");
		delay(150);
	}

	lcd.clear();
}


/*
* GESTIONA EL MODO FIJO
*/
void modeFixed()
{
	// se desactiva cualquier temporización existente
//	Timer1.detachInterrupt();

//	Timer2.detachInterrupt();
//	Timer2.attachInterrupt(ISR_Stop).setFrequency(config.duration * 60 * 1000).start();

	// se abren las dos zonas de riego durante el parametro duration
	huerto.RiegoOn();
	campo.RiegoOn();
	tanque.On();
	
}

/*
* GESTIONA EL MODO RIEGO SEGÚN SENSORES
*/

void modeDripIrrigation()
{
	// se desactiva cualquier temporización existente
	Timer1.detachInterrupt();
	Timer2.detachInterrupt();
	
	// Zona Huerto. Entre rango de humedad, temperatura que evita congelación y no llueve
	if ( strcmp(huerto.ESTADO_HUM, "DRY") == 0 && strcmp(sky.ESTADO_TEMP, "GOOD") == 0 && !sky.rain )
		huerto.RiegoOn();
	else
		huerto.RiegoOff();

	// Zona Campo. Entre rango de humedad, temperatura que evita congelación y no llueve
	if ( strcmp(campo.ESTADO_HUM, "DRY") == 0 && strcmp(sky.ESTADO_TEMP, "GOOD") == 0 && !sky.rain )
		campo.RiegoOn();
	else
		campo.RiegoOff();

	// En caso de que se haya abierto una de las zonas se abre el tanque
	// la clase Tank gestiona si se abre la red de augua o la bomba del tanque
	if (huerto.riego || campo.riego)	tanque.On();
	else								tanque.Off();
}

/*
* GESTIONA EL RIEGO TEMPORIZADO
*/
void modeTimer()
{
	// se activa tiempo de espera hasta nuevo riego
	// Dispara cada intervalo en horas * 3600s * 1000ms
	// y activa la interrupcion y la asocia a ISR_Start
	Timer1.stop();
	Timer1.attachInterrupt(ISR_Start).setFrequency(config.interval * 3600 * 1000).start();
}

void ISR_Start()
{
	Serial.println("ISR_Start");
	//  Se activa tiempo de riego
	// Dispara cada intervalo en horas * 60s * 1000ms
	Timer2.detachInterrupt();
	Timer2.attachInterrupt(ISR_Stop).setFrequency(config.duration * 60 * 1000).start();

	// arranca riego
	huerto.RiegoOn();
	campo.RiegoOn();
	tanque.On();
}

void ISR_Stop()
{
	Serial.println("ISR_Stop");

	// Se desactiva la interrupción   
	Timer2.stop();

	// se detiene riego
	tanque.Off();
	huerto.RiegoOff();
	campo.RiegoOff();
}
