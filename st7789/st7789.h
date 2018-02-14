/*
 * cfaf240320a032t.h
 *
 * Created: 2015-08-18 9:16:09 AM
 *  Author: max
 */ 

#ifndef __ST7789_H__
#define __ST7789_H__

#include "../atmega2561/atmega2561.h"

// color definitions
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void clearScreen(void);
void displayHome(void);
void displayInit(void);
void enterSleep (void);
void exitSleep (void);
void fillScreen(unsigned long color);
void readID(void);
void setDisplayWindow(int x0, int y0, int x1, int y1);
void setInterface(void);
void writeColorBars(void);
//
// Customer Code starts here
//

#endif /* __CFAF240320H1032T_H__ */
