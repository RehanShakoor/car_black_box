/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : timer.c
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//This file contains function definitions related to timers

//include required headers
#include "main.h"

//function definition to initialize timer1
//timer1 is used to check user block delay
void timer1_init(void)
{
    //clear TMR1ON (bit0), to disable timer1
    T1CON &= 0xFE;
    
    //clear TMR1CS (bit1), to select internal clock source
    T1CON &= 0xFD;
    
    //T1CKPS(1-0) (bit5-4) (10), to select 1:4 pre-scale value
    T1CON &= 0xCF;
    T1CON |= BIT5;
    
    //to get a time delay of 40 MS per timer1 overflow
    TMR1 = 15536;
}

//function definition to initialize timer1 interrupt
void timer1_interrupt_init(void)
{
    //set GIE (bit7), to enable global interrupt
    //set PEIE (bit6), to enable peripheral interrupt
    INTCON |= 0xC0;

    //set TMR1IE (bit0), to enable timer1 interrupt
    PIE1 |= BIT0;
    
    //clear TMR1IF (bit0), to clear timer1 interrupt flag
    PIR1 &= 0xFE;
}