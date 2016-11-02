/*
	Modified spi_LTC1448 example for LCD
	based on ILI9341 controller: June 2015
	by Vencislav Atanasov (user890104)

	Updated for the new firmware: July 2012
	by ihsan Kehribar <ihsan@kehribar.me>

	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
*/

#include <stdio.h>
#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "littleWire.h"
#include "littleWire_util.h"

#include "spi.h"

unsigned char version;

#define CD_CMD digitalWrite(lw, PIN3, LOW)
#define CD_DATA digitalWrite(lw, PIN3, HIGH)

int main()
{
	littleWire *lw = NULL;

	lw = littleWire_connect();

	if (lw == NULL) {
		printf("> Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}

	version = readFirmwareVersion(lw);
	printf("> Little Wire firmware version: %d.%d\n", (version & 0xF0) >> 4, version & 0x0F);
	
	if (version == 0x10)
	{
		printf("> This example requires the new 1.1 version firmware. Please update soon.\n");
		exit(EXIT_FAILURE);
	}
	
	pinMode(lw, PIN3, OUTPUT); // D/C
	CD_CMD;
	
	spi_init(lw);
	spi_updateDelay(lw, 0);
	
	unsigned char cmd[1];
	
	cmd[0] = 0x01;
	spi_rw(lw, cmd, NULL, 1);
	
	Sleep(5);

	cmd[0] = 0x04;
	spi_rw(lw, cmd, NULL, 1);

	unsigned char id[4];
	spi_rw(lw, NULL, id, 4);
	printf("ID: 0x%02x 0x%02x 0x%02x 0x%02x\n", id[0], id[1], id[2], id[3]);
	printf("ID: %u %u %u %u\n", id[0], id[1], id[2], id[3]);
	
	return 0;
}
