
/**
* @file     ESP8266.c
* @brief    This file contanis routines to configure,
*           send and receive data thorugh wifi module ESP8266.
* @author   Mehdi
*
*/

#ifndef F_CPU
#define F_CPU 7372800UL
#endif

#include <string.h>
#include <stdlib.h>

#include "AuxLib.h"
#include "UART_4.h"
#include "LCD.h"

#include "ESP8266.h"



/**
 * @name    ESP_SET
 * @brief   The function send AT commands to ESP to configure it
 *
 * @author  Mehdi
*/

void ESP_SET(void)
{

    char *Response;
    uint8_t CheckRes;
    uint8_t Rec_Status;

    LCDClear();

    // Disconnect from the router
    LCDWriteStringXY(0,0,"DISCONNECT FROM AP");
    Rec_Status = ESP_SendCommand("AT+CWQAP\r\n",1);
    Response = ESP_GetResponse(Rec_Status,1);    // Read serial Data
    CheckRes = ESP_CheckResponse(Response,"WIFI DISCONNECT");  // Check the response and send back a feedback.
    ActAcorToRes(CheckRes);
    USART_RxBufferFlush();
    dely_s(2);
    LCDClear();

    // Configure as access point
    LCDWriteStringXY(0,0,"SET as ACCESS POINT");
    Rec_Status = ESP_SendCommand("AT+CWMODE=1\r\n",1);
    Response = ESP_GetResponse(Rec_Status,3);
    CheckRes = ESP_CheckResponse(Response,"OK");
    ActAcorToRes(CheckRes);
    USART_RxBufferFlush();
    dely_s(2);
    LCDClear();

    // Establish connection to the Router
    LCDWriteStringXY(0,0,"CONNECT to ROUTER");
    ESP_ConnectToRouter();
    dely_s(2);
    LCDClear();

    // Get the Static IP assigned by Router
    LCDWriteStringXY(0,0,"GET the STATIC IP");
    ESP_GetIP();
    dely_s(2);
    LCDClear();

    // Configure for multiple connections
    LCDWriteStringXY(0,0,"CONFIG for MPL CONN.");
    Rec_Status = ESP_SendCommand("AT+CIPMUX=1\r\n",1);
    Response = ESP_GetResponse(Rec_Status,3);
    CheckRes = ESP_CheckResponse(Response,"OK");
    ActAcorToRes(CheckRes);
    USART_RxBufferFlush();
    dely_s(2);
    LCDClear();

    // Turn on server on port 80
    LCDWriteStringXY(0,0,"TURN on SERVER&P.80");
    Rec_Status = ESP_SendCommand("AT+CIPSERVER=1,80\r\n",1);
    Response = ESP_GetResponse(Rec_Status,3);
    CheckRes = ESP_CheckResponse(Response,"OK");
    ActAcorToRes(CheckRes);
    USART_RxBufferFlush();
    dely_s(2);
    LCDClear();

}


/**
 * @name    ESP_Init
 * @brief   The function send command "AT" to initialize the module.
 *
 * @author  Mehdi
 */

 void ESP_Init(void)
 {

    char CheckRes;
    char* CmdRes;
    uint8_t Rec_Status;

        // Reset the ESP
/*    LCDWriteStringXY(0,0,"RESET the MODULE");
    Rec_Status = ESP_SendCommand("AT+RST\r\n",2);
    USART_RxBufferFlush();
    dely_s(6);
    LCDClear();*/

    // Initialize the module
    LCDWriteStringXY(0,0,"INITIALIZATION");
    Rec_Status = ESP_SendCommand("AT\r\n",1);
    CmdRes = ESP_GetResponse(Rec_Status,1);    // Read serial Data
    CheckRes = ESP_CheckResponse(CmdRes,"OK");  // Check the response and send back a feedback.
    ActAcorToRes(CheckRes);

    USART_RxBufferFlush();
    dely_s(2);
 }

 /**
 * @name    ESP_ConnectToRouter
 * @brief   The function Make connection between module and intented router
 *
 * @author  Mehdi
 */

 void ESP_ConnectToRouter(void)
 {

    char *Response = "";
    uint8_t Rec_Status;

    Rec_Status = ESP_SendCommand("AT+CWJAP=\"Ciel\",\"1703198328\"\r\n",3);
    dely_s(10);

    Response = ESP_GetResponse(Rec_Status,1);

    if ((strstr(Response,"WIFI CONNECTED") != NULL) && (strstr(Response,"WIFI GOT IP") != NULL))     // Search to find specific String in the response
    {
        LCDWriteStringXY(0,2,"> ESTABLISHED");
    }else
    {
        LCDWriteStringXY(0,2,"> NOT ESTABLISHED");
        dely_s(3);
        Halt();
    }

    USART_RxBufferFlush();

 }

 /**
 * @name    ESP_GetIP
 * @brief   The function Get the Static IP from the module
 *              assigned by the router connected to and print it on LCD.
 *
 * @author  Mehdi
 */

 void ESP_GetIP(void)
 {

    char *tmp = "";
    char *Static_IP = "";
    char *Response = "";
    uint8_t Rec_Status;

    Rec_Status = ESP_SendCommand("AT+CIFSR\r\n",1);	// Get IP address
    Response = ESP_GetResponse(Rec_Status,5);

    if (strstr(Response,"STAIP") != NULL)
    {
        tmp = strstr(Response,"STAIP");
        strncpy(Static_IP,tmp,17);        // Copy data after "STAIP."<IP>"" in a string in order to print in LCD

        LCDWriteStringXY(0,2,Static_IP);
    } else
    {
        LCDWriteStringXY(0,2,"Unable to Get IP!");
        dely_s(3);
        Halt();
    }
    USART_RxBufferFlush();

 }


/**
 * @name    ESP_SendCommand
 * @brief   The function send AT commands to ESP to
 *              configure it and receive the response
 *
 * @author  Mehdi
 *
 * @param	Command: AT command
 * @param	delayTime: delay time
 * @param	Conf_Status: the results of sending commands
 */

uint8_t ESP_SendCommand (char* Command , uint8_t delayTime)
{

    char counter;
    uint8_t Rec_Status = 0;

    USART_Transmit_String(Command);

    for (counter = 0; counter < 10;counter++)
    {
        if (USART_DataAvailable(delayTime) == USART_DATARECEIVED)
        {
            Rec_Status = 1;
            break;
        }
    }

    return Rec_Status;
}


/**
 * @name    ESP_GetResponse
 * @brief   The function gets the response from UART
 *              buffer and send it back to function calls it
 *
 * @author  Mehdi
 *
 * @param	Rec_Status: Receiving status 1: Data Received   0: Data Not Received
 * @param	delayTime: delay uC waits for receiving the reply.
 * @return  Response: Data received through UART
 */

char * ESP_GetResponse (uint8_t Rec_Status, uint8_t delayTime)
{

    char *Response = "";

    if ((Rec_Status == 0))
    {
        Response = "NULL";
    } else if (Rec_Status == 1)
    {
        Response = USART_Receive_String_ISR();
    }

    return Response;
}


/**
 * @name    ESPCheckResponse
 * @brief   The function check the response received from module
 *              and return back appropriate feedback.
 *              it first echoes the cmd sent by uC and then the appropriate response
 *
 * @author  Mehdi
 *
 * @param	response: the response get from module and check by the function
 * @param   check: the word with witch "response" is compared
 * @return  ESP8266_INVALID_RESPONSE: if two first or two last char of response are not "\r\n"
 * @return  ESP8266_FAIL: if "response" does not match "check"
 * @return  ESP8266_OK: if the response matches check
*/

char ESP_CheckResponse(const char *response, const char *check)
{

    if (strcmp(response,"NULL") == 1)
    {
        return ESP8266_FAIL;
    }else if (strstr(response,check) == NULL)
    {
        return ESP8266_INVALID_RESPONSE;
    } else if (strstr(response,check) != NULL)
    {
        return ESP8266_OK;
    }
}


/**
 * @name    ActAcorToRes
 * @brief   The function print appropriate text on LCD according
 *              to the response given from ESP_CheckResponse
 *
 * @author  Mehdi
 *
 * @param   Res: the response get from module and check by the function
 */

void ActAcorToRes(char Res)
{

    if (Res == ESP8266_INVALID_RESPONSE)
    {
        LCDWriteStringXY(0,2,"> INVALID RESPONSE");
        dely_s(3);
        Halt();
    }else if (Res == ESP8266_FAIL)
    {
        LCDWriteStringXY(0,2,"> NO MATCH RESPONSE");
        dely_s(3);
        Halt();
    }else if (Res == ESP8266_OK)
    {
        LCDWriteStringXY(0,2,"> RESPONSE OK");
    }
}


/**
 * @name    ESP_SendHTTPResponse
 * @brief   Function that creates and sends following HTTP response
 *
 * @author  Mehdi
 *
 * @param	ConnectionID: the connection ID sent by android device
 * @param	Response: the data created by uC based on the arbitrary action
*/

void ESP_SendHTTPResponse(char* ConnectionID, char* Response)
{
	// build HTTP response
    char* HttpResponse = "";

    HttpResponse = Response;	// Append the main message (e.g. Light 1 in ON) created by uC to HTTP Header

    ESP_SendCIPData(ConnectionID,HttpResponse);
}


/**
 * @name    ESP_SendCIPData
 * @brief   sends a CIPSend=<connectionId>,<data> command
 *              and then the HTTP Response created and given by
 *              ESP_SendHTTPResponse
 *
 * @author  Mehdi
 *
 * @param	ConnectionID: the connection ID sent by android device
 * @param	HttpResponse: The Statement prepared by the "SendHTTPResponse" function
*/

void ESP_SendCIPData(char* ConnectionID, char* HttpResponse)
{
    char strdata[4];
    char CheckRes;
    char* CmdRes;
    uint8_t Rec_Status;
    char* CIPSendCmd = "AT+CIPSEND=";

//    itoa(strlen(HttpResponse),strdata,10);		// Get the length of a char and convert it to string
    sprintf(strdata,"%d",strlen(HttpResponse));    // Get length of data and convert it into string

    CIPSendCmd = Concat(CIPSendCmd,ConnectionID);
    CIPSendCmd = Concat(CIPSendCmd,",");

    CIPSendCmd = Concat(CIPSendCmd,strdata);
    CIPSendCmd = Concat(CIPSendCmd,"\r\n");

    // Send "AT+CIPSEND=<Connection ID>,<Number of Char>"
    ESP_SendCommand(CIPSendCmd,1);
    CmdRes = ESP_GetResponse(Rec_Status,1);    // Read serial Data

    CheckRes = ESP_CheckResponse(CmdRes,"OK");  // Check the response and send back a feedback.

    ActAcorToRes(CheckRes);

    USART_RxBufferFlush();

    // Send the message
    ESP_SendCommand(HttpResponse,1);
    CmdRes = ESP_GetResponse(Rec_Status,1);    // Read serial Data

    CheckRes = ESP_CheckResponse(CmdRes,"SEND OK");  // Check the response and send back a feedback.

    ActAcorToRes(CheckRes);

    USART_RxBufferFlush();

    LCDClear();
}

/**
 * @name    Send_Close_Command
 * @brief   Create and sends a close command to the module
 *              AT+CIPCLOSE=<ConnectionID>
 *
 * @author  Mehdi
 *
 * @param	ConnectionID: the connection ID sent by android device
*/

void ESP_SendCloseCommand (char* ConnectionID)
{
    char* CloseCommand;
    char CheckRes;
    char* CmdRes;
    uint8_t Rec_Status;

    // Make close command
    CloseCommand = "AT+CIPCLOSE=";
    CloseCommand = Concat(CloseCommand,ConnectionID); // Append Connection ID

    CloseCommand = Concat(CloseCommand,"\r\n");

    // Send AT+CIPCLOSE=<Connection ID> to Close the Connection
    ESP_SendCommand(CloseCommand,1);
    CmdRes = ESP_GetResponse(Rec_Status,1);    // Read serial Data

    CheckRes = ESP_CheckResponse(CmdRes,"CLOSED");  // Check the response and send back a feedback.

    ActAcorToRes(CheckRes);
    USART_RxBufferFlush();

    LCDClear();
}

