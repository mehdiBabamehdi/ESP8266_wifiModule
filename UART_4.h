

#ifndef USARTLIB_H_
#define USARTLIB_H_

/*_________________________________________________________________________________________*/

#include <avr/io.h>


//************************************************

#define USART_DATARECEIVED	1
#define USART_TIMEOUT		0

#define Rec_Data_Length	 256	// The maximum length of the data will be received  and stored by uC

#define TX_BUFFER_SIZE 256	// Length of buffer for transmitter

#define RX_BUFFER_SIZE 256	// Length of buffer for Receiver

volatile char Received_Data[RX_BUFFER_SIZE]; // The data in which receiving data is stored

volatile char Transmitted_Data[TX_BUFFER_SIZE]; // The data in which transmitting data is stored


/***************************************************
			F U N C T I O N S
		   I N I T I A L I Z A T I O N
****************************************************/

void USART_Initialization(long  Baud_Rate, char Data_Bits, char Parity, char Stop_Bits, char AsyncDoubleSpeed);

void USART_Interrupt_Int (char Rec_Comp_Int, char Tran_Comp_Int, char Data_Reg_Empty_Int);


/***************************************************
			F U N C T I O N S
			R E C E I V E R
****************************************************/

char USART_Receive_char(void);

unsigned char* USART_Receive_String(void);

char USART_Receive_char_ISR(void);

char* USART_Receive_String_ISR(void);

uint8_t USART_DataAvailable(uint8_t timeOut);

uint8_t USART_LenRecData(void);

void USART_RxBufferFlush (void);


/***************************************************
			  F U N C T I O N S
			T R A N S M I T T E R
****************************************************/

void USART_Transmit_char(char data);

void USART_Transmit_String(char* StringPtr);

void USART_Transmit_char_ISR (char Transmitted_DATA);


#endif	/* USARTLIB_H_	*/


