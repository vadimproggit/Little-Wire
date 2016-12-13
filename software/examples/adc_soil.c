/*
	Modified to turn off soil sensor when not in use: Dec 2016
	by Vencislav Atanasov <user89@gmail.com>
	
	Updated for the new firmware: July 2012
	by ihsan Kehribar <ihsan@kehribar.me>
	
	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"

int main(int argc, char **argv)
{
	littleWire *lw = NULL;

	lw = littleWire_connect();

	if (lw == NULL) {
		fprintf(stderr, "> Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}

	unsigned char version = readFirmwareVersion(lw);
	fprintf(stderr, "> Little Wire firmware version: %d.%d\n", ((version & 0xF0) >> 4), (version & 0x0F));	
	
	if (version == 0x10) {
		fprintf(stderr, "> This example requires the new 1.1 version firmware. Please update soon.");
		return 0;
	}
	
	analog_init(lw, VREF_VCC);

	while(1) {
		digitalWrite(lw, PIN4, HIGH);
		delay(1000);
		
		unsigned int adcValue = analogRead(lw, ADC_PIN2);
		
		digitalWrite(lw, PIN4, LOW);
		
		float voltage = (float)adcValue * 5000 / 1024.0;
	
		fprintf(stderr, "> Value: %X\tVoltage: %f milivolts\n", adcValue, voltage);
		printf("%f\n", voltage);
		
		if (lwStatus < 0) {
			fprintf(stderr, "> lwStatus: %d\n",lwStatus);
			fprintf(stderr, "> Connection error!");
			return 0;
		}
		
		delay(1000);
	}
}
