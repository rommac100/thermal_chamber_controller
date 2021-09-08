#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include "temp_control.h"


// Backlight colors in case that wants to be changed.
#define RED 0x1
#define GREEN 0x2
#define YELLOW 0x3 
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

/*
 * LCD State Table
 * S0 = Status Display
 * S1 = Main Menu
 * S2 = Temp Control
 * S3 = Emergency Stop Display
 */

#define STATUS_DISPLAY 0 
#define MAIN_MENU 1
#define TEMP_CONTROL 2
#define EMERGENCY_MENU 3

uint8_t lcd_state;
uint8_t prev_lcd_state;

// main_menu vars
uint8_t main_menu_state;
uint8_t cursor_row;

/*
 * Main Menu States
 * S0 = Change_Temp & Stop Heating
*/

#define S0_MAIN_MENU 0

void init_lcd()
{
	lcd.begin(16,2);
	lcd_state = STATUS_DISPLAY;
}

uint8_t button_check()
{
	return lcd.readButtons();
}

void print_status_display(float curr_temp, float set_temp, uint8_t temp_state)
{
	
	lcd.setBacklight(YELLOW);
	char temp[6];
	lcd.setCursor(0,0);
	lcd.print("C_Temp: ");
	lcd.print(dtostrf(curr_temp,4,1,temp));

	if (temp_state == ACTIVE_HEATING)
		lcd.print(" H");
	else 
		lcd.print(" O");

	lcd.setCursor(0,1);
	lcd.print("S_Temp: ");
	lcd.print(dtostrf(set_temp,4,1,temp));
}

void state_trans(uint8_t button_state)
{
	if (button_state)
	{
		if (lcd_state == STATUS_DISPLAY && (button_state& BUTTON_SELECT))
		{
			prev_lcd_state = STATUS_DISPLAY;
			lcd_state = MAIN_MENU;
			lcd.clear();
			main_menu_state = S0_MAIN_MENU;
			cursor_row = 0;
		}
		else if (lcd_state == MAIN_MENU && main_menu_state == S0_MAIN_MENU)
		{
			if (button_state&BUTTON_SELECT)
			{
				lcd.noBlink();
				prev_lcd_state = MAIN_MENU;
				lcd_state = STATUS_DISPLAY;
				lcd.clear();
			}
			else if (button_state&BUTTON_RIGHT)
			{
				switch (main_menu_state)
				{
					case S0_MAIN_MENU:
						if (cursor_row == 1){
							prev_lcd_state = MAIN_MENU;
							lcd_state = EMERGENCY_MENU;
						}
					break;
				}
			}
		}
	}
}

void print_main_menu(uint8_t button_state)
{
	switch(main_menu_state)
	{
		case S0_MAIN_MENU:
			lcd.setBacklight(YELLOW);
			lcd.setCursor(0,0);
			lcd.print("Set Temp");
			lcd.setCursor(0,1);
			lcd.print("EMERGENCY_STOP");
			if (button_state&BUTTON_DOWN)
			{
				cursor_row+=(button_state !=1) ?1:0;
			}
			else if (button_state&BUTTON_UP)
			{
				cursor_row-= (cursor_row !=0) ?1:0;
			}
			lcd.setCursor(15,cursor_row);
			lcd.blink();
		break;
	}
}

void print_emergency_stop()
{
	lcd.setBacklight(RED);
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("EMERGENCY HALT");
}

void refresh_display(float curr_temp, float set_temp, uint8_t heating_state)
{
	uint8_t button_state = button_check();
	state_trans(button_state);
	switch (lcd_state)
	{
		case STATUS_DISPLAY:
			print_status_display(curr_temp,set_temp,heating_state);
		break;
		case MAIN_MENU:
			print_main_menu(button_state);
		break;
		case EMERGENCY_MENU:
			print_emergency_stop();
		break;
	}
}
