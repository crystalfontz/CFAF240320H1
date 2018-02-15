/*
 * cfaf240320a032t.h
 *
 * Created: 2015-08-18 9:16:09 AM
 *  Author: max
 */ 

#include <avr/io.h>
#ifndef __ST7789_H__
#define __ST7789_H__

// color definitions
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

extern void writeCommand(uint8_t command);
extern void writeData(uint8_t data);
extern void writeColor(uint16_t color);
extern uint8_t readData(void);
extern void clrCS(void);
extern void setCS(void);
extern void clrRS(void);
extern void setRS(void);
extern void clrWR(void);
extern void setWR(void);
extern void clrRD(void);
extern void setRD(void);
extern void clrRESET(void);
extern void setRESET(void);
extern void uPInit(char* interfaceType);


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
