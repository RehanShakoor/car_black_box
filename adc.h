/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : adc.h
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//This file contains function declarations and macros related to ADC

#ifndef ADC_H
#define	ADC_H

//include required headers
#include "main.h"

//define acquisition time in us
#define ACQ_DELAY 20

//function declaration to initialize ADC
void adc_init(void);

//function declaration to read ADC value
uint16_t adc_read(void);

#endif	/* ADC_H */

