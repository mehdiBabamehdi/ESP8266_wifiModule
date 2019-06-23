/*
* Name: General Definitions library
* Author: Mehdi
* Version: 01
* Description:
*				The library was developed to introduce constants, Bitwise Operations and
*				the auxiliary routines which have general purpose.
*/

#ifndef F_CPU
#define F_CPU 7372800UL
#endif

#include <string.h>
#include <util/delay.h>
#include <stdlib.h>

#include "AuxLib.h"
#include "LCD.h"


/**
 * Name: Find_Char
 * Description: The function tries to find char "PIN" in the received string
 * @Author: Mehdi
 *
 * @Params		String: the string in which three following chars whould be found
 * @Params		Num_Array: Index of char "P" in the received string
 * @Params		crt: The chars should be found in the received string
*/
char* Find_Char(char* String, char crt1, char crt2, char crt3)
{
	char *pch1, *pch2, *pch3;

	pch1 = strchr(String,crt1);	// Find the index of char P

	while(pch1 != NULL)
	{
		pch2 = strchr(pch1+1,crt2);
		if (pch1 == pch2 - 1)	// Find the index of char i
		{
			pch3 = strchr(pch2+1,crt3);	// Find the index of char n

			if (pch2 == pch3 -1)
			{
				break;
			}
		}
		pch1 = strchr(pch1+1,crt1);
	}
	return pch3+2;
}

/**
 * Name: Find_Str
 * Description: The function tries to find specific substring in the received
 *		string and then returns string according to specific order 
 *		(step of jump forward, and length of string)
 * @Author: Mehdi
 *
 * @Params		Str1: The string in which str2 whould be found
 * @Params		str2: The string should be looked for in str1
 * @Params		JumpStep: Number of char jump through str1 to find intended char
 * @Params		LengthofRet: Length of chars found and returned by function				
*/

char* Find_Str(char* str1, char* str2, int JumpStep, int LengthofRet)
{
    char* pchr;

	char* res = malloc(LengthofRet+1);

	pchr = strstr(str1,str2);
	strncpy(res,pchr+JumpStep,LengthofRet);
	res[LengthofRet]='\0';

	return (res);
}


/**
 * Name: Concat
 * Description: The Routine Concatenates two strings.
 * @Author: Mehdi
 *
 * @Params	str1 & str2; the strings that Concatenated in the function
 * @Params	result: the string results from Concatenation of the received string
 */
char* Concat(char* str1, char* str2)
{
	char* result = malloc(strlen(str1)+strlen(str2)+1);	// +1 for the zero-terminator

	strcpy(result, str1);
	strcat(result, str2);

	return result;
}


/**
 * Name: Halt
 * Description: The Routine halts the program.
 * @Author: Mehdi
 */
void Halt(void)
{    
	volatile int j;	
	LCDClear();
	LCDWriteStringXY(5,1,"E R R O R");
	while(1)
	{
		dely_s(3);
		j++;
	}
}

/*
 * Name: dely_s
 * Description: The Routine imposes delay in second
 * @Author: Mehdi
 *
 * @Params	time: intended delay time in second
 */

void dely_s(uint16_t timeS)
{
	volatile int j=0;
	for (int i = 0 ; i < timeS * 50 ; i++) {
		_delay_ms(20);
		j++;
	}
}


/*
 * Name: dely_ms
 * Description: The Routine imposes delay in second
 * @Author: Mehdi
 *
 * @Params	time: intended delay time in milisecond
 */

void dely_ms(uint16_t timeMS)
{
	volatile int j=0;
	for (int i = 0 ; i < timeMS ; i++) {
		_delay_ms(1);
		j++;
	}
}

