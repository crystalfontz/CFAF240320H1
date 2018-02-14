//===========================================================================
//
//
//  CRYSTALFONTZ 
//
//  This code uses the 8-bit parallel MCU mode of the display.
//
//  https://www.crystalfontz.com/product/cfah1604atmijt-character-display-16x4
//
//  The controller is a Sitronix ST7066U:
//    https://www.crystalfontz.com/controllers/Sitronix/ST7066U
//
//  Seeeduino v4.2, an open-source 3.3v capable Arduino clone.
//    https://www.seeedstudio.com/Seeeduino-V4.2-p-2517.html
//    https://github.com/SeeedDocument/SeeeduinoV4/raw/master/resources/Seeeduino_v4.2_sch.pdf
//============================================================================
//
//
//
//===========================================================================
//This is free and unencumbered software released into the public domain.
//
//Anyone is free to copy, modify, publish, use, compile, sell, or
//distribute this software, either in source code form or as a compiled
//binary, for any purpose, commercial or non-commercial, and by any
//means.
//
//In jurisdictions that recognize copyright laws, the author or authors
//of this software dedicate any and all copyright interest in the
//software to the public domain. We make this dedication for the benefit
//of the public at large and to the detriment of our heirs and
//successors. We intend this dedication to be an overt act of
//relinquishment in perpetuity of all present and future rights to this
//software under copyright law.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.
//
//For more information, please refer to <http://unlicense.org/>
//============================================================================



//============================================================================
// LCD & USD control lines
//   ARD      | Port |			  |  Function					| Wire
//------------+------+------------+-----------------------------+------------
//  5V	      |		 | #2         |  POWER 5V					| Red
//  GND	      |		 | #1 #2 #31  |  GROUND						| Black
//  3.3V      |		 | #13-15     |  POWER 3.3V					| Green
//  GND	      |		 | #16        |  GROUND						| Black
//  N/A	      |		 | #3         |  POWER 1V					| Orange
// -----------+------+------------+-----------------------------+------------
//  #8/D8     |  PB0 | #4         |  Chip Enable Signal (CS)	| Purple
//  #9/D9     |  PB1 | #5         |  Data/Instruction   (RS)	| Yellow
// #10/D10    |  PB2 | #6         |  Read/Write         (WR)	| Grey
// #10/D11    |  PB2 | #7         |  Read/Write         (RD)	| Brown
// #11/D12    |  PB2 | #28        |  Reset		        (RESET) | Blue
// -----------+------+------------+-----------------------------+------------
//  #0/D0     |  PD0 | #20        |  LCD_D10 (DB0)				| Purple
//  #1/D1     |  PD1 | #21        |  LCD_D11 (DB1)				| Green
//  #2/D2     |  PD2 | #22        |  LCD_D12 (DB2)				| Orange
//  #3/D3     |  PD3 | #23        |  LCD_D13 (DB3)				| Yellow
//  #4/D4     |  PD4 | #24        |  LCD_D14 (DB4)				| Grey
//  #5/D5     |  PD5 | #25        |  LCD_D15 (DB5)				| Brown
//  #6/D6     |  PD6 | #26        |  LCD_D16 (DB6)				| White
//  #7/D7     |  PD7 | #27        |  LCD_D17 (DB7)				| Black
// -----------+------+------------+-----------------------------+------------
//============================================================================

#define CLR_CS     (PORTB &= ~(0x01)) //pin #8  - Chip Enable Signal
#define SET_CS     (PORTB |=  (0x01)) //pin #8  - Chip Enable Signal
#define CLR_RS     (PORTB &= ~(0x02)) //pin #9  - Data/Instruction
#define SET_RS     (PORTB |=  (0x02)) //pin #9  - Data/Instruction
#define CLR_WR	   (PORTB &= ~(0x04)) //pin #10 - Read/Write
#define SET_WR	   (PORTB |=  (0x04)) //pin #10 - Read/Write
#define CLR_RD	   (PORTB &= ~(0x08)) //pin #11 - 
#define SET_RD	   (PORTB |=  (0x08)) //pin #11 - 
#define CLR_RESET  (PORTB &= ~(0x10)) //pin #12 - Reset
#define SET_RESET  (PORTB |=  (0x10)) //pin #12 - Reset

//#include "st7789/st7789.c"
//#include "arduino/arduino.c"
#include <SoftwareSerial.h>
SoftwareSerial mySerial(14, 13);


void writeCommand(uint8_t command)
{
	CLR_CS;
	CLR_RS;
	SET_RD;
	SET_WR;


	PORTD = command&0xFF;

	CLR_WR;
	SET_WR;
	SET_CS;
}

void writeData(uint8_t data)
{
	SET_RS;


	PORTD = data&0xFF;

	CLR_WR;
	SET_WR;
}

uint8_t readData()
{
	CLR_CS;
	SET_RS;
	SET_RD;
	SET_WR;
	uint8_t response = 0x00;
	PORTD = 0x00;
	DDRD = 0x00;
	//DDRD = 0xFF;
	delay(1);
	CLR_RD;
	SET_RD;
	response = PIND;

	DDRD = 0xFF;
	//DDRD = 0x00;

	SET_CS;
	return response;
}

void init_8080()
{
	CLR_RD;
	CLR_WR;
	CLR_CS;
	SET_RS;
	PORTD = 0x00;

	CLR_RESET;
	delay(1000);
	SET_RESET;
	delay(5000);

	//--------------------------------ST7789S Frame rate setting----------------------------------// 
	//Porch Setting
	writeCommand(0xb2);
		//	  1011 0010
	writeData(0x0c);
		// 0000 1100
		// 0XXX XXXX
		//	||| ||||--	BPA6:BPA0
		//				  Back porch setting in normal mode. Minimum setting is 0x01

	writeData(0x0c);
		// 0000 1100
		// 0XXX XXXX
		//	||| ||||--	FPA6:FPA0
		//				  Front porch setting in normal mode. Minimum setting is 0x01

	writeData(0x00);
		// 0000 0000
		// 0000 000X
		//		   |--	PSEN
		//				>>0: Disable separate porch control
		//				  1: Disable separate porch control
	writeData(0x33);
		// 0011 0011
		// XXXX XXXX
		// |||| ||||--	FPB3:FPB0
		// ||||			  Front porch setting in idle mode. The minimum setting is 0x01
		// ||||
		// ||||-------	BPB3:BPB0
		//				  Back porch setting in idle mode. The minimum setting is 0x01
	writeData(0x33);
		// 0011 0011
		// XXXX XXXX
		// |||| ||||--	FPC3:FPC0
		// ||||			  Front porch setting in partial mode. The minimum setting is 0x01
		// ||||
		// ||||-------	BPC3:BPC0
		//				  Back porch setting in partial mode. The minimum setting is 0x01
	//Gate Control
	writeCommand(0xb7);
		//	  1011 0111
	writeData(0x35);
		// 0011 0101
		// 0XXX 0XXX
		//  |||	 |||--	VGL Setting
		//  |||			  000: -7.16
		//  |||			  001: -7.67
		//  |||			  010: -8.23
		//  |||			  011: -8.87
		//  |||			  100: -9.6
		//  |||			>>101: -10.43
		//  |||			  110: -11.38
		//  |||			  111: -12.5
		//  |||
		//  |||-------	VGH Setting
		//  			  000: 12.2
		//  			  001: 12.54
		//  			  010: 12.89
		//  			>>011: 13.26
		//  			  100: 13.65
		//  			  101: 14.06
		//  			  110: 14.5
		//  			  111: 14.97
	//---------------------------------ST7789S Power setting--------------------------------------// 
	writeCommand(0x3a);
	writeData(0x05);

	writeCommand(0x36);
	writeData(0x00);

	writeCommand(0xbb);
	writeData(0x2b);//vcom 

	writeCommand(0xc2);
	writeData(0x01);

	writeCommand(0xc3);//gvdd
	writeData(0x0b); //����ǳ12

	writeCommand(0xc4);
	writeData(0x20);

	writeCommand(0xc6);
	writeData(0x0f);

	writeCommand(0xd0);
	writeData(0xa4);
	writeData(0xa2);
	//--------------------------------ST7789S gamma setting---------------------------------------// 
	writeCommand(0xe0);
	writeData(0xd0);
	writeData(0x00);
	writeData(0x03);
	writeData(0x08);
	writeData(0x0a);
	writeData(0x17);
	writeData(0x2e);
	writeData(0x44);
	writeData(0x3f);
	writeData(0x29);
	writeData(0x10);
	writeData(0x0e);
	writeData(0x14);
	writeData(0x18);

	writeCommand(0xe1);
	writeData(0xd0);
	writeData(0x00);
	writeData(0x03);
	writeData(0x08);
	writeData(0x0a);
	writeData(0x17);
	writeData(0x2e);
	writeData(0x44);
	writeData(0x3f);
	writeData(0x29);
	writeData(0x10);
	writeData(0x0e);
	writeData(0x14);
	writeData(0x18);

	writeCommand(0x2A); //Frame rate control
	writeData(0x00);
	writeData(0x00);
	writeData(0x00);
	writeData(0xEF);

	writeCommand(0x2B); //Display function control
	writeData(0x00);
	writeData(0x00);
	writeData(0x01);
	writeData(0x3F);

	writeCommand(0x11);
	delay(120);      //Delay 120ms 

	writeCommand(0x29); //display on
	writeCommand(0x2c);

	//set display window
	uint8_t x0 = 0;
	uint8_t x1 = 240;
	uint8_t y0 = 0;
	uint8_t y1 = 320;
	writeCommand(0x2a);	// Column Address Set
	writeData(x0 >> 8);	// X address start:
	writeData(x0);		// 0 <= XS <= X
	writeData(x1 >> 8);	// X address end:
	writeData(x1);		// S <= XE <= X

	writeCommand(0x2b);	// Row Address Set
	writeData(y0 >> 8);	// Y address start:
	writeData(y0);		// 0 <= YS <= Y
	writeData(y1 >> 8);	// Y address start:
	writeData(y1);		// S <= YE <= Y
}

void setup()
{
	DDRD = 0xFF;
	PORTD = 0x00;
	DDRB = 0x3F;
	mySerial.begin(9600);
	for (int i = 0; i < 100; i++)
	{
		mySerial.println();
	}
	mySerial.println("new started setup");
	int i = 0;
	//while (1)
	//{
	//	SET_CS;
	//	delay(50);
	//	CLR_CS;
	//	delay(50);
	//}

	init_8080();

	
		mySerial.print("loop #: " );
		mySerial.println(i);
		mySerial.println("data: ");
		writeCommand(0x04);
		mySerial.print("dummy line: ");
		mySerial.println(readData(), HEX);
		mySerial.print("ID1 Read: ");
		mySerial.println(readData(), HEX);
		mySerial.print("ID2 Read: ");
		mySerial.println(readData(), HEX);
		mySerial.print("ID3 Read: ");
		mySerial.println(readData(), HEX);
		delay(5000);
	
		
	mySerial.println("init completed");
}

void returnHome()
{
	writeCommand(0x20);//Horizontal Address
	writeData(0x00);
	writeCommand(0x21);//Vertical Address
	writeData(0x00);
}


#define	BLACK	  0x0000
#define	BLUE	  0x001F
#define	RED 	  0xF800
#define	GREEN	  0x07E0
#define CYAN	  0x07FF
#define MAGENTA	  0xF81F
#define YELLOW	  0xFFE0
#define WHITE	  0xFFFF



void loop()
{
	mySerial.println("loop started");
	unsigned int i, j;
	returnHome();
	//writeCommand(0x22);

	for (i = 0; i<320; i++)
	{
		for (j = 0; j<240; j++)
		{
			if (i > 279)
			{
				writeData(BLACK >> 8);
				writeData(BLACK);
			}
			else if (i > 239)
			{
				writeData(BLUE >> 8);
				writeData(BLUE);
			}
			else if (i > 199)
			{
				writeData(GREEN >> 8);
				writeData(GREEN);
			}
			else if (i > 159)
			{
				writeData(CYAN >> 8);
				writeData(CYAN);
			}
			else if (i > 119)
			{
				writeData(RED >> 8);
				writeData(RED);
			}
			else if (i > 79)
			{
				writeData(MAGENTA >> 8);
				writeData(MAGENTA);
			}
			else if (i > 39)
			{
				writeData(YELLOW >> 8);
				writeData(YELLOW);
			}
			else
			{
				writeData(WHITE >> 8);
				writeData(WHITE);
			}
		}
	}
	delay(1000);

}
