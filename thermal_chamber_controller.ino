#include <Arduino.h>
#include "libraries/lcd_control.h"
//#include "libraries/temp_control.hg

void setup()
{
	init_lcd();
}

void loop()
{
	delay(500);	
	refresh_display(999.9f,999.9f,INACTIVE_HEATING);
}
