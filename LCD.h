/******************************************************************************

                                    LCD Core
						           ----------
This module is used for driving Standard Alpha Numeric LCD Modules.

*******************************************************************************/

#include <avr/io.h>

#ifndef _LCD_H
#define _LCD_H


/*_________________________________________________________________________________________*/

#include "config.h"


//************************************************

#define LS_BLINK 0B00000001
#define LS_ULINE 0B00000010
#define LS_NONE	 0B00000000



/***************************************************
			F U N C T I O N S
****************************************************/



void LCDInit(uint8_t style);

void LCDWriteString(const char *msg);
void LCDWriteFString(const char *msg);

void LCDWriteInt(int val,int8_t field_length);
void LCDGotoXY(uint8_t x,uint8_t y);

//Low level
void LCDByte(uint8_t,uint8_t);
#define LCDCmd(c) (LCDByte(c,0))
#define LCDData(d) (LCDByte(d,1))

void LCDBusyLoop(void);


/***************************************************
			F U N C T I O N S     E N D
****************************************************/


/***************************************************
	M A C R O S
***************************************************/
#define LCDClear() LCDCmd(0b00000001)
#define LCDHome() LCDCmd(0b00000010);

#define LCDWriteStringXY(x,y,msg) {\
 LCDGotoXY(x,y);\
 LCDWriteString(msg);\
}

#define LCDWriteFStringXY(x,y,msg) {\
LCDGotoXY(x,y);\
LCDWriteFString(msg);\
}

#define LCDWriteIntXY(x,y,val,fl) {\
 LCDGotoXY(x,y);\
 LCDWriteInt(val,fl);\
}
/***************************************************/




/*_________________________________________________________________________________________*/
#endif
