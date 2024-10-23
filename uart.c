/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : uart.c
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//This file contains function definitions related to UART

//include required headers
#include "main.h"

//function definition to initialize UART transmit
void uart_tx_init(void)
{
    //set BRGH (bit2), to use high baud rate
    TXSTA |= BIT2;
    
    //set value to SPBRG, to get 9600 baud rate 
    //FOSC is 20 MHz, actual baud rate generated is 9615 (error 0.16 %)
    SPBRG = 129;
     
    //set TXEN (bit5), to enable transmit
    TXSTA |= BIT5;
    
    //set SPEN (bit7), to enable serial port pins (RC7/RX, RC6/TX)
    RCSTA |= BIT7;
}

//function definition to transmit data using UART
void uart_tx_data(uint8_t data)
{
    //wait till TXREG register is empty
    //TXIF (bit4) is set when TXREG is empty
    while(!(PIR1 & BIT4));

    //send data to TXREG register
    TXREG = data;
}

//function definition to transmit string using UART
void uart_tx_string(char *str)
{
    //iterate over string
    while(*str)
    {
        //send current char and increment pointer
        uart_tx_data(*str);
        str = str + 1;
    }
}