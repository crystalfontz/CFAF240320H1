#include "arduino.h"
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

// **************************************************
void displayHome(void)
{
	writeCommand(0x2c); // Memory write
	// When this command is accepted, the column register and the page
	// register are reset to the start column/start page positions.
}
// **************************************************
void pictureSlideShow()
{
	uint8_t  i;              //loop variable
	uint16_t j;              //loop variable
	uint32_t p;              //cluster
	uint16_t *buffer;        //buffer
	uint16_t pics = 1;
	uint16_t totalFiles = 0;
	uint16_t slideshowFlag = 1;
	uint8_t  sector;
	uint32_t pixels;
	char * PATH = "\\batch";

	Search(PATH, &PictureInfo, &totalFiles);

	if (totalFiles == 0)
	{
		return;
	}

	if(!(buffer = malloc(512)))
	{
		return;
	}
	
	do{
		clearScreen(); // BLACK
		//find the file
		Search(PATH, &PictureInfo, &pics);
		
		//the first cluster of the file
		p = PictureInfo.deStartCluster + (((uint32_t)PictureInfo.deHighClust) << 16);
		
		sector = 0;

		//read a sector
		FAT_LoadPartCluster(p, sector, buffer);

		// total # of pixels to fill
		pixels = (uint32_t) 320 * 240;

		// byte count
		j = 0; 
	
		while(pixels > 0)
		{
			writeColor(buffer[j]);  // write 16 bits
			pixels--;               // which is one pixel
			
			j++;                    // increment word count
			if (j == 256)           // time for a new sector
			{
				sector++;
				if (sector == SectorsPerClust)
				{
					p = FAT_NextCluster(p);	// read next cluster
					sector = 0;
				}

				FAT_LoadPartCluster(p, sector, buffer);	// read a sector
				j = 0;
			}
		}

		if(slideshowFlag)
		{
			for (i = 0; i < 3; i++)	// delay for a while
			{
				delay(0xFFFF);
			}

			pics++;					// increment picture number
			if (pics > totalFiles)	// if last
			{
				pics = 1;			// wrap around
			}

		}
	} while(slideshowFlag);
	
	free(buffer);
}
// **************************************************
uint8_t readData(void)
{
	uint8_t data;

	LED2_ON
	SET_RS
	PORTD = 0x00; //Clear PORTE
	DDRD = 0x00; //Set DDRE to input
	CLR_RD
	SET_RD
	delay(100);
	data = PINE; // read from the port
	DDRE = 0xFF; //Set DDRE to output
	delay(10);
	LED2_OFF

	return data;
}
// **************************************************
void reset_display(void)
{
	CLR_RESET
	delay(10);
	SET_RESET
	delay(10);
	SET_RD
}
// **************************************************
void writeCommand(unsigned long command)
{
	LED1_ON
	CLR_RS
#ifdef MCU16BIT
	PORTE=command>>8;
	PORTA=command;
#endif
#ifdef MCU8BIT
	//SET_WR;
	//PORTE=0x00;
	//
	//CLR_WR;
	//SET_WR;

	PORTE=command;
#endif
	CLR_WR
	SET_WR
	delay(10);
	LED1_OFF
}
// **************************************************
void writeData(unsigned long data)
{
	LED2_ON
	SET_CD
	#ifdef MCU16BIT
	PORTE=data>>8;
	PORTA=data;
	#endif
	#ifdef MCU8BIT
	//SET_WR;
	//PORTE=0x00;
	//
	//CLR_WR;
	//SET_WR;

	PORTE=data;
	#endif
	CLR_WR
	SET_WR
	delay(10);
	LED2_OFF
}
// **************************************************
void writeColor(unsigned long color)
{
	LED2_ON
	SET_CD
#ifdef MCU16BIT	
	PORTE=color>>8;
	PORTA=color;
#endif
#ifdef MCU8BIT
 	CLR_WR
 	PORTE=color>>8;
 	SET_WR
	PORTE=color;
#endif
	CLR_WR
	SET_WR
	LED2_OFF
}
