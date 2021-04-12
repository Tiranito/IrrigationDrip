/**
* MUESTRA EL SUBMENU EN EL LCD.
*
* @param menuID    ID del menu principal para usarlo como titulo del submenu
* @param screen    Segun el tipo, se representara el submenu de una forma u otra.
* @param value     Puntero a la variable que almacena el dato, y que se modificara.
* @param minValue  Valor minimo que puede tener la variable.
* @param maxValue  Valor maximo que puede tener la variable.
*/
void openMenu()
{
	byte idxMenu = 0;
	boolean exitMenu = false;
	boolean forcePrint = true;

	huerto.RiegoOff();
	campo.RiegoOff();
	lcd.clear();

	while (!exitMenu)
	{
		btnPressed = readButtons();

		if (btnPressed == Button::Left && idxMenu - 1 >= 0)
		{
			idxMenu--;
		}
		else if (btnPressed == Button::Right && idxMenu + 1 < iMENU)
		{
			idxMenu++;
		}
		else if (btnPressed == Button::Ok)
		{
			switch (idxMenu)
			{
			case 0: openSubMenu(idxMenu, Screen::Menu1, &config.mode, 0, COUNT(txSMENU1) - 1); break;
			case 1: openSubMenu(idxMenu, Screen::Number, &config.hum_suelo_min, 0, 100); break; // %
			case 2: openSubMenu(idxMenu, Screen::Number, &config.hum_suelo_max, 0, 100); break; // %
			case 3: openSubMenu(idxMenu, Screen::Number, &config.hum_huerto_min, 0, 100); break; // %
			case 4: openSubMenu(idxMenu, Screen::Number, &config.hum_huerto_max, 0, 100); break; // %
			case 5: openSubMenu(idxMenu, Screen::Menu2, &config.temp_unit, 0, COUNT(txSMENU2) - 1); break;
			case 6: openSubMenu(idxMenu, Screen::Number, &config.temp_min, 0, 50); break; // grados
			case 7: openSubMenu(idxMenu, Screen::Number, &config.temp_max, 0, 50); break; // grados
			case 8: openSubMenu(idxMenu, Screen::Number, &config.interval, 0, 48); break; // horas
			case 9: openSubMenu(idxMenu, Screen::Number, &config.duration, 0, 30); break; // minutos
			case 10: openSubMenu(idxMenu, Screen::Number, &config.timezone, -2, 2); break; // minutos
			case 11: config.Write(); exitMenu = true; break; //Salir y guardar
			case 12: config.Read();  exitMenu = true; break; //Salir y cancelar cambios
			}
			forcePrint = true;
		}

		if (!exitMenu && (forcePrint || btnPressed != Button::Unknown))
		{
			forcePrint = false;

			static const byte endFor1 = (iMENU + rowsLCD - 1) / rowsLCD;
			int graphMenu = 0;

			for (int i = 1; i <= endFor1; i++)
			{
				if (idxMenu < i * rowsLCD)
				{
					graphMenu = (i - 1) * rowsLCD;
					break;
				}
			}

			byte endFor2 = graphMenu + rowsLCD;

			for (int i = graphMenu, j = 0; i < endFor2; i++, j++)
			{
//				u8g2.drawStr(1, j, (i < iMENU) ? txMENU[i] : "                    ");
				lcd.setCursor(1, j);
				lcd.print((i < iMENU) ? txMENU[i] : "                    ");
			}

			for (int i = 0; i < rowsLCD; i++)
			{
//				u8g2.drawStr(0, i, " ");
				lcd.setCursor(0, i);
				lcd.print(" ");
			}
//			u8g2.drawStr(0, idxMenu % rowsLCD, ">");
			lcd.setCursor(0, idxMenu % rowsLCD);
			lcd.print(">"); // arrow
//			u8g2.sendBuffer();					// transfer internal memory to the display
		}
	}

	lcd.clear();
}


/**
MUESTRA EL SUBMENU EN EL LCD.

@param menuID    ID del menu principal para usarlo como titulo del submenu
@param screen    Segun el tipo, se representara el submenu de una forma u otra.
@param value     Puntero a la variable que almacena el dato, y que se modificara.
@param minValue  Valor minimo que puede tener la variable.
@param maxValue  Valor maximo que puede tener la variable.
*/
void openSubMenu(byte menuID, int screen, byte *value, int minValue, int maxValue)
{
	boolean exitSubMenu = false;
	boolean forcePrint = true;

	lcd.clear();

	while (!exitSubMenu)
	{
		 btnPressed = readButtons();

		if (btnPressed == Button::Ok)
		{
			exitSubMenu = true;
		}
		else if (btnPressed == Button::Left && (*value) - 1 >= minValue)
		{
			(*value)--;
		}
		else if (btnPressed == Button::Right && (*value) + 1 <= maxValue)
		{
			(*value)++;
		}


		if (!exitSubMenu && (forcePrint || btnPressed != Button::Unknown))
		{
			forcePrint = false;

//			u8g2.drawStr(0, 0, txMENU[menuID]);
			lcd.setCursor(0, 0);
			lcd.print(txMENU[menuID]);

//			u8g2.drawStr(0, 1, "<");
			lcd.setCursor(0, 1);
			lcd.print("<");
//			u8g2.drawStr(columnsLCD - 1, 1, ">");
			lcd.setCursor(columnsLCD - 1, 1);
			lcd.print(">");

			if (screen == Screen::Menu1)
			{
//				u8g2.drawStr(1, 1, txSMENU1[*value]);
				lcd.setCursor(1, 1);
				lcd.print(txSMENU1[*value]);
			}
			else if (screen == Screen::Menu2)
			{
//				u8g2.drawStr(1, 1, txSMENU2[*value]);
				lcd.setCursor(1, 1);
				lcd.print(txSMENU2[*value]);
			}
			else if (screen == Screen::Flag)
			{
//				u8g2.drawStr(columnsLCD / 2 - 1, 1, *value == 0 ? "NO" : "SI");
				lcd.setCursor(columnsLCD / 2 - 1, 1);
				lcd.print(*value == 0 ? "NO" : "SI");
			}
			else if (screen == Screen::Number)
			{
//				u8g2.drawStr(columnsLCD / 2 - 1, 1, *value);
				lcd.setCursor(columnsLCD / 2 - 1, 1);
				lcd.print(*value);
				lcd.print(" ");
			}
		}
	}
	lcd.clear();
}



/**
LEE LOS DISTINTOS BOTONES DISPONIBLES Y DEVUELVE EL QUE HAYA SIDO PULSADO
*/
int readButtons()
{
	static boolean oldA = HIGH;
	static boolean newA = LOW;
	static boolean newB = LOW;

	btnPressed = Button::Unknown;
	newA = digitalRead(pENCO_DT);
	newB = digitalRead(pENCO_CLK);

	if (!oldA && newA)
	{
		btnPressed = !newB ? Button::Left : Button::Right;
		delay(50);
	}
	else if (!digitalRead(pENCO_SW))
	{
		while (!digitalRead(pENCO_SW));
		btnPressed = Button::Ok;
		delay(50);
	}

	oldA = newA;
	return btnPressed;
}


