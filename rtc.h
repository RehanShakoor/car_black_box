/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : rtc.c
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//This file contains function declarations and macros related to RTC

#ifndef RTC_H
#define	RTC_H

//define RTC read and write address (taken from DS1307 data-sheet)
#define RTC_WRITE_ADDRESS 0xD0
#define RTC_READ_ADDRESS  0xD1

//function declaration to set value of RTC (DS1307)
void rtc_set_time(uint8_t sec, uint8_t min, uint8_t hr);

//function declaration to read and update time from RTC (DS1307)
void rtc_read_time(void);

//function declaration to convert BCD to decimal
//values read from RTC are in BCD form
uint8_t bcd_to_decimal(uint8_t bcd);

//function declaration to convert decimal to BCD
//values stored in RTC must be in BCD form
uint8_t decimal_to_bcd(uint8_t dec);

#endif	/* RTC_H */

