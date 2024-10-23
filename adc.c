/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : adc.c
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */


//This file contains function definitions related to ADC

//include required header
#include "main.h"

//function definition to initialize ADC
void adc_init(void)
{
    //clear PCFG3:0 (bit3:0), to select analog input function on pins
    //Vref+ is Vdd and Vref- is Vss
    ADCON1 &= 0xF0;
    
    //clear CHS2:0 (bit5:3), to select channel0
    ADCON0 &= 0xC7;
    
    //ADCS2:0 (110), to select Fosc/64 clock source
    //ADCS2 is in ADCON1 bit6 (set it)
    //ADCS1:0  is in ADCON0 (bit7-6) (10)
    ADCON1 |= BIT6;
    ADCON0 &= 0x3F;
    ADCON0 |= BIT7;
    
    //set ADFM (bit7), to select right justified result
    ADCON1 |= BIT7;
    
    //set ADON (bit0), to turn on ADC
    ADCON0 |= BIT0;
    
    //clear GO (bit2), to don't start the ADC conversion
    ADCON0 &= 0xFB; 
}


//function definition to read ADC value
uint16_t adc_read(void)
{
    //wait for acquisition time
    __delay_us(ACQ_DELAY);
        
    //set GO (bit2), to start the ADC conversion
    ADCON0 |= BIT2;
    
    //wait for ADC conversion to complete
    //GO (bit2) clear on ADC conversion
    while(ADCON0 & BIT2);
        
    //store ADC conversion result in a variable
    //lower 8 bits of result is stored in ADRESL
    //higher 2 bits of result is stored in ADRESH
    
    //return read ADC value
    return (((uint16_t) ADRESH) << 8) | ((uint16_t) ADRESL);
}