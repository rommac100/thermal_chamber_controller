#include <Arduino.h>
#include "libraries/lcd_control.h"
//#include "libraries/temp_control.hg

float curr_temp;
float set_temp;
void setup()
{
	init_lcd();
	curr_temp =0.0f;
	set_temp = 240.0f;
}

void loop()
{
	delay(500);	
	refresh_display(curr_temp,set_temp,INACTIVE_HEATING);
	curr_temp += (curr_temp != set_temp) ? 1.0f : 0;
	
}
