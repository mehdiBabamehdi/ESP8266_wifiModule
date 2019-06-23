/*
 * ESP8266_wifi.c
 * Ver: 1.0
 * Created: 8/28/2016 12:19:52 PM
 * Author : Mehdi
 */

#define F_CPU 7372800UL

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "AuxLib.h"
#include "UART_4.h"
#include "LCD.h"

#include "ESP8266.h"

void ADC_Init(void);
char* ADCon(void);


char* ESP_GetData;		// The data received by ESP


int main(void)
{
	char* num_char;
	char* ConnectionID;
	char* Room_Temp;
	char* Response;
	char* Greeting_msg = "Hello World!";


	USART_Initialization(115200,8,NONE,1,0);	// Initialize the UART; Baud Rate=115.2k, 8-byte data size, No parity, one stop bit, disable Double Speed in Asynchronization
	USART_Interrupt_Int(TRUE,FALSE,FALSE);		// Initialization of USART Interrupt (RXC, TXC, UDRE)

	ADC_Init();		// Initialization of ADC

	LCDInit(LS_BLINK|LS_ULINE);	// Initialize LCD module, LCD Blink & Cursor is "underline" type

	DDRB  |= 1 << PINB0 | 1 << PINB1 | 1 << PINB2;
	PORTB &= ~(1 << PINB0);
	PORTB &= ~(1 << PINB1);
	PORTB &= ~(1 << PINB2);


	ESP_Init();     // Sent Cmd "AT" and wait for response
	USART_RxBufferFlush();

	ESP_SET();      // Set the module as AP, Connect to router, Get IP, and server on port 80

	LCDWriteStringXY(0,1,"C O N F I G U R E D");
	dely_s(3);
	LCDClear();	

/*	Room_Temp = ADCon();
	LCDWriteStringXY(0,1,"Tempreature = ");
	LCDWriteString(Room_Temp);
	LCDWriteString(" C");
	dely_s(3);
	LCDClear();*/

	LCDWriteStringXY(4,1,"Waiting . . .");
	PORTB |= 1 << PINB0;
	dely_s(1);


    while (1)
    {
		if (USART_DataAvailable(1) == USART_DATARECEIVED)
		{
            ESP_GetData = USART_Receive_String_ISR();		// Get the string sent by Android device
            PORTB ^= 1 << PINB0;
            LCDClear();

            if (Find_Str(ESP_GetData,"IPD",4,1) != NULL)
            {
                ConnectionID = Find_Str(ESP_GetData,"IPD",4,1);	// Get the connection ID sent by the Android device
                num_char = Find_Str(ESP_GetData,"pin",4,4);

                if (strcmp(num_char, "SL1E") == 0)
                {
                    PORTB ^= 1 << PINB1;
                    if (PORTB & (1 << PINB1))
                    {
                        Response = "Light 1 is ON";
                    } else
                    {
                        Response = "Light 1 is OFF";
                    }

                    ESP_SendCIPData(ConnectionID,Response);
                    //dely_s(5);
                    ESP_SendCloseCommand(ConnectionID);
                    //dely_s(5);
                    USART_RxBufferFlush();

                } else if (strcmp(num_char, "SL2E") == 0)
                {
                    PORTB ^= 1 << PINB2;
                    if (PORTB & (1 << PINB2))
                    {
                        Response = "Light 2 is ON";
                    } else
                    {
                        Response = "Light 2 is OFF";
                    }

                    ESP_SendCIPData(ConnectionID,Response);
                    //dely_s(5);
                    ESP_SendCloseCommand(ConnectionID);
                    //dely_s(5);
                                USART_RxBufferFlush();
                } else if (strcmp(num_char, "ST1E") == 0)
                {
                    LCDWriteString("Request For Room Temperature!");
                    Response = "Temp: ";		// C stands for Centigrade
                    Room_Temp = ADCon();	// Reading Room Temp.
                    Response = strcat(Response,Room_Temp);
                    Response = strcat(Response," C");

                    ESP_SendCIPData(ConnectionID,Response);
                    dely_s(5);
                    ESP_SendCloseCommand(ConnectionID);
                    dely_s(5);
                                USART_RxBufferFlush();
                } else
                {
                    LCDWriteString("RESPONSE NOT MATCH");
                                USART_RxBufferFlush();
                }
            }else if (Find_Str(ESP_GetData,"IPD",4,1) == NULL){
                LCDWriteString("INVALID RESPONSE");
            }

            LCDClear();
            LCDWriteStringXY(4,1,"Waiting . . .");

        }
    }
}



/**
 * Name: ADC_Init
 * Description: The Routine Set Initial Conditions for ADC
 * @Author: Mehdi
 */
void ADC_Init(void)
{
	ADCSRA |= 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0; // ADC PRESCALAR; CK/128
	ADMUX  |= 1 << REFS0;		// AREF = AVcc
	ADMUX  |= 1 << ADLAR;		// SET 10-BIT RESOLUTION, Left-Justifies
	ADCSRA |= 1 << ADEN;		// ENABLE ADC
}


/**
 * Name: ADCon
 * Description: The Routine Read the Temp. from Sensor and Convert Analog to Digital, 10-bit, left-justified
 * @Author: Mehdi
 *
 * @Params	tenBitvalue: the temp. sensed by analog sensor and convert to digital
 * @Return	Temp_Char: 10-bit temperature in string
 */
char* ADCon(void)
{
	uint16_t ADCTemp;
	int Data_Accuracy = 4;
	char* Temp_Char = malloc(Data_Accuracy + 1);

	ADCSRA |= 1 << ADSC;	// Start Conversion

	while(!(ADCSRA & (1 << ADIF)));	// Wait to complete ADC

	uint8_t theLow = ADCL;
	uint16_t tenBitvalue = ADCH << 2 | theLow >> 6;

	ADCTemp = tenBitvalue*0.432;	// ADC*Vref/2^bit_size/Temp. scale(LM35 Temp. scale = 10 mV/C)
	itoa(ADCTemp,Temp_Char,10);		// Convert integer to string
	Temp_Char[4] = '\0';

	return(Temp_Char);
}

