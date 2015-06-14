/*
	Modified spi_LTC1448 example for touch screens
	based on ADS7843/XPT2045 controller: June 2015
	by Vencislav Atanasov (user890104)

	Updated for the new firmware: July 2012
	by ihsan Kehribar <ihsan@kehribar.me>

	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"

unsigned char version;
unsigned short touchX, touchY;

#define CMD_START 0x80
#define CMD_A2 0x40
#define CMD_A1 0x20
#define CMD_A0 0x10
#define CMD_MODE_12BIT 0
#define CMD_MODE_8BIT 0x08
#define CMD_REF_SINGLE 0x04
#define CMD_REF_DIFF 0
#define CMD_PD1 0x02
#define CMD_PD0 0x01

#define CMD_BASE CMD_START | CMD_MODE_12BIT | CMD_REF_SINGLE

// CALIBRATION DETAILS, TOUCH PANEL SPECIFIC
// FIND THE EDGE VALUES FOR YOUR PANEL, THEN SET THEM HERE
#define TS_TOP 0x1bf
#define TS_BOTTOM 0xd7f
#define TS_LEFT 0x1b0
#define TS_RIGHT 0xe3c

unsigned short readData(littleWire *lw, unsigned char cmd) {
	unsigned char sendBuffer[3];
	unsigned char receiveBuffer[3];	
	
	sendBuffer[0] = CMD_BASE | cmd;
	sendBuffer[1] = 0;
	sendBuffer[2] = 0;
	
	spi_sendMessage(lw, sendBuffer, receiveBuffer, 3, MANUAL_CS);
	
	if (lwStatus < 0)
	{
		printf("> lwStatus: %d\n",lwStatus);
		printf("> Connection error!\n");
		exit(EXIT_FAILURE);
	}
	
	return receiveBuffer[1] << 5 | receiveBuffer[2] >> 3;
}

int main()
{
	// disable stdout buffering
	setbuf(stdout, NULL);
	
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
	
	spi_init(lw);
	spi_updateDelay(lw, 0);
	
	while (1)
	{
		// TODO: can be combined in a single transfer
		touchX = readData(lw, CMD_A0);
		touchY = readData(lw, CMD_A2 | CMD_A0);
		
		// These values mean "no pen touches the screen"
		if (touchX == 0xfff && touchY == 0x00) {
			continue;
		}

		if (touchX < TS_LEFT || touchX > TS_RIGHT || touchY < TS_TOP || touchY > TS_BOTTOM) {
			// maybe show warning about value out of range?
			continue;
		}

		if (printf(
			"%u %u\r\n",
			320 * (touchX - TS_LEFT) / (TS_RIGHT - TS_LEFT),
			240 * (touchY - TS_TOP) / (TS_BOTTOM - TS_TOP)
		) < 0) {
			// exit on stdout pipe closed
			break;
		}
	}
	
	return 0;
}
