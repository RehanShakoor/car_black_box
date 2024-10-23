/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : timer.h
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//This file contains function declarations and macros related to timers


#ifndef TIMER_H
#define	TIMER_H

//include required headers
#include "main.h"

//function declaration to initialize timer1
//timer1 is used to check user block delay
void timer1_init(void);

//function declaration to initialize timer1 interrupt
void timer1_interrupt_init(void);

#endif	/* TIMER_H */

