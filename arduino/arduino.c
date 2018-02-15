#include "arduino.h"

//#include "../st7789/st7789.h"


void uPInit(char* interfaceType)
{
	if(interfaceType == "8-bit parallel")
	{
		DDRD = 0xFF;
		PORTD = 0x00;
		DDRB = 0x3F;
	}
}
// **************************************************
void displayHome(void)
{
	writeCommand(0x2c); // Memory write
	// When this command is accepted, the column register and the page
	// register are reset to the start column/start page positions.
}
// **************************************************

uint8_t readData(void)
{
	uint8_t data;

	SET_RS;
	PORTD = 0x00; //Clear PORTE
	DDRD = 0x00; //Set DDRE to input
	CLR_RD;
	SET_RD;
	delay(100);
	data = PIND; // read from the port
	DDRD = 0xFF; //Set DDRE to output
	delay(10);

	return data;
}
// **************************************************
void reset_display(void)
{
	CLR_RESET;
	delay(10);
	SET_RESET;
	delay(10);
	SET_RD;
}
// **************************************************
void writeCommand(uint8_t command)
{
	CLR_RS;
	SET_WR;

	PORTD = command & 0xFF;

	CLR_WR;
	SET_WR;
}
// **************************************************
void writeData(uint8_t data)
{
	SET_RS;
	SET_RD;

	PORTD = data & 0xFF;

	CLR_WR;
	SET_WR;
}
// **************************************************
void writeColor(uint16_t color)
{
	SET_RS;
 	PORTD=color>>8;
	CLR_WR;
	SET_WR;
	PORTD=color;
	CLR_WR;
	SET_WR;
}

void clrCS(void) { CLR_CS; };
void setCS(void) { SET_CS; };
void clrRS(void) { CLR_RS; };
void setRS(void) { SET_RS; };
void clrWR(void) { CLR_WR; };
void setWR(void) { SET_WR; };
void clrRD(void) { CLR_RD; };
void setRD(void) { SET_RD; };
void clrRESET(void) { CLR_RESET; };
void setRESET(void) { SET_RESET; };