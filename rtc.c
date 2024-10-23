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

//This file contains function definitions related to RTC

//include required headers
#include "main.h"

//function definition to set value of RTC (DS1307)
void rtc_set_time(uint8_t sec, uint8_t min, uint8_t hr)
{
    //call function to generate i2c start condition
    //0,1,2 are for start/restart/stop respectively 
    i2c_control(0);
    
    //send RTC write address
    i2c_write(RTC_WRITE_ADDRESS);
    
    //send address of RTC where we want to write
    //send 0x00 to start writing from seconds
    i2c_write(0x00);
    
    //set time as 00:00:00, send BCD values
    i2c_write(decimal_to_bcd(sec));
    i2c_write(decimal_to_bcd(min));
    i2c_write(decimal_to_bcd(hr));
    
    //call function to generate i2c stop condition
    //0,1,2 are for start/restart/stop respectively 
    i2c_control(2);
}

//function definition to read and update time from RTC (DS1307)
void rtc_read_time(void)
{
    //call function to generate i2c start condition
    //0,1,2 are for start/restart/stop respectively 
    i2c_control(0);
    
    //send RTC write address
    i2c_write(RTC_WRITE_ADDRESS);
    
    //send address of RTC from where we want to read
    //send 0x00 to start reading from seconds
    i2c_write(0x00);
    
    //call function to generate i2c restart condition
    //0,1,2 are for start/restart/stop respectively 
    i2c_control(1);
    
    //send RTC read address
    i2c_write(RTC_READ_ADDRESS);
    
    //variables to store time
    uint8_t seconds = 0, minutes = 0, hours = 0;
    
    //read seconds (register 0x00)
    //send ACK after reading
    seconds = i2c_read(1);  
    
    //read minutes (register 0x01)
    //send ACK after reading
    minutes = i2c_read(1);  
    
    //read hours (register 0x02)
    //send NACK after reading the last byte
    hours = i2c_read(0);  
    
    //call function to generate i2c stop condition
    //0,1,2 are for start/restart/stop respectively 
    i2c_control(2);
    

    //convert seconds to decimal
    seconds = bcd_to_decimal(seconds);
    //update seconds in global time variable (ASCII value)
    current_time[6] = (seconds / 10) + 48;
    current_time[7] = (seconds % 10) + 48;
    
    //convert minutes to decimal
    minutes = bcd_to_decimal(minutes);
    //update minutes in global time variable (ASCII value)
    current_time[3] = (minutes / 10) + 48;
    current_time[4] = (minutes % 10) + 48;
    
    //convert hours to decimal
    hours = bcd_to_decimal(hours);
    //update hours in global time variable (ASCII value)
    current_time[0] = (hours / 10) + 48;
    current_time[1] = (hours % 10) + 48;
}

//function definition to convert BCD to decimal
//values read from RTC are in BCD form
uint8_t bcd_to_decimal(uint8_t bcd)
{
    //in BCD, upper nibble represents tens place of number
    //and lower nibble represents ones place of number
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

//function definition to convert decimal to BCD
//values stored in RTC must be in BCD form
uint8_t decimal_to_bcd(uint8_t dec)
{
    //in BCD, upper nibble represents tens place of number
    //and lower nibble represents ones place of number
    return (uint8_t) (((dec / 10) << 4)) | (dec & 0x0F);
}