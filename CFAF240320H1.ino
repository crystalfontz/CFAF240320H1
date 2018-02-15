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




//set the path of the controller
#define controllerPath "../arduino/arduino.h"

#include "st7789/st7789.c"
#include "arduino/arduino.c"

#include <SoftwareSerial.h>


SoftwareSerial mySerial(14, 13);



void setup()
{
	mySerial.begin(9600);
	for (int i = 0; i < 100; i++)
	{
		mySerial.println();
	}
	mySerial.println("new started setup");
	int i = 0;


	displayInit();

	
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
	
		char* interfaceType = "8-bit parallel";
	mySerial.println("init completed");
	mySerial.print("interfaceType: ");
	mySerial.println(interfaceType);
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
	//returnHome();
	//writeCommand(0x22);
	fillScreen(WHITE);
	delay(1000);
	writeColorBars();
	delay(1000);

}
