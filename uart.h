/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : uart.h
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//This file contains function declarations and macros related to UART

#ifndef UART_H
#define	UART_H

//include required headers
#include "main.h"

//function declaration to initialize UART transmit
void uart_tx_init(void);

//function declaration to transmit data using UART
void uart_tx_data(uint8_t data);

//function declaration to transmit string using UART
void uart_tx_string(char *str);


#endif	/* UART_H */

