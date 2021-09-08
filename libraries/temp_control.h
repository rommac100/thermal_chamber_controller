//pin definitions
#define HEATER_PIN 1
#define FAN_PIN 2
#define TEMP_SEN_PIN A1


// States
#define INACTIVE_HEATING 0
#define ACTIVE_HEATING 1

void setup_thermal()
{

}

uint16_t read_temp()
{
	//uint16_t raw = analogRead(TEMP_SEN_PIN);
	return 0;
}
