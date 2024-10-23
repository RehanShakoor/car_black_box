/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : eeprom.c
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//This file contains function definitions related to external and internal eeprom
//External EEPROM is used to store event logs
//Internal EEPROM is used to store password

//include required headers
#include "main.h"

//define struct varible to store event info
struct event_format event_info;

//function definition to write external EEPROM
void ext_eeprom_write(uint8_t addr, uint8_t data)
{
    //call function to generate i2c start condition
    //0,1,2 are for start/restart/stop respectively
    i2c_control(0);
    
    //send EEPROM device write address
    i2c_write(EEPROM_WRITE_ADDRESS);
    
    //send address to be written on EEPROM
    i2c_write(addr);
    
    //send data to be written on EEPROM
    i2c_write(data);
    
    //call function to generate i2c stop condition
    //0,1,2 are for start/restart/stop respectively
    i2c_control(2);
}

//function definition to read external EEPROM
uint8_t ext_eeprom_read(uint8_t addr)
{
    //call function to generate i2c start condition
    //0,1,2 are for start/restart/stop respectively
    i2c_control(0);
    
    //send EEPROM device write address
    i2c_write(EEPROM_WRITE_ADDRESS);
    
    //send address to read from EEPROM
    i2c_write(addr);
    
    
    //call function to generate i2c restart condition
    //0,1,2 are for start/restart/stop respectively
    i2c_control(1);
    
    //send EEPROM device read address
    i2c_write(EEPROM_READ_ADDRESS);
    
    //call function to read i2c data and send NACK
    uint8_t data = i2c_read(0);
    
    //call function to generate i2c stop condition
    //0,1,2 are for start/restart/stop respectively
    i2c_control(2);
    
    //return read data
    return data;
}

//function definition to read event info from eeprom
void read_event_eeprom(uint8_t eeprom_mem_addr_read)
{
    //read seconds from given address of eeprom
    uint8_t seconds = ext_eeprom_read(eeprom_mem_addr_read);
    //read minutes from given address of eeprom
    uint8_t minutes = ext_eeprom_read(eeprom_mem_addr_read + 1);
    //read hours from given address of eeprom
    uint8_t hours = ext_eeprom_read(eeprom_mem_addr_read + 2);
    
    //store time info in global event info struct
    //update seconds 
    event_info.event_time[6] = (seconds / 10) + 48;
    event_info.event_time[7] = (seconds % 10) + 48;
    event_info.event_time[8] = '\0';
    
    //update minutes
    event_info.event_time[3] = (minutes / 10) + 48;
    event_info.event_time[4] = (minutes % 10) + 48;
    event_info.event_time[5] = ':';

    //update hours 
    event_info.event_time[0] = (hours / 10) + 48;
    event_info.event_time[1] = (hours % 10) + 48;
    event_info.event_time[2] = ':';


    //read event signature from given eeprom address
    event_info.event_signature[0] = ext_eeprom_read(eeprom_mem_addr_read + 3);
    event_info.event_signature[1] = ext_eeprom_read(eeprom_mem_addr_read + 4);
    event_info.event_signature[2] = '\0';


    //read event speed from given eeprom address
    event_info.event_speed = ext_eeprom_read(eeprom_mem_addr_read + 5);
}

//function definition to store captured event in external eeprom
//this will store time, event and speed
void store_event_eeprom(void)
{   
    //return if eeprom is full
    if(eeprom_end_addr == 252)
    {
        return;
    }

    //retrieve seconds from global time variable
    uint8_t seconds = ((current_time[6] - 48) * 10) + (current_time[7] - 48);
    //write seconds at given address of EEPROM
    ext_eeprom_write(eeprom_end_addr, seconds);
    
    //retrieve minutes from global time variable
    uint8_t minutes = ((current_time[3] - 48) * 10) + (current_time[4] - 48);
    //write minutes at next address of EEPROM
    ext_eeprom_write(eeprom_end_addr + 1, minutes);
    
    //retrieve hours from global time variable
    uint8_t hours = ((current_time[0] - 48) * 10) + (current_time[1] - 48);
    //write hours at next address of EEPROM
    ext_eeprom_write(eeprom_end_addr + 2, hours);    
    

    //write first char of last event at next address of EEPROM
    ext_eeprom_write(eeprom_end_addr + 3, last_event[0]);
    //write first char of last event at next address of EEPROM
    ext_eeprom_write(eeprom_end_addr + 4, last_event[1]);
    
    
    //write current speed at next address of EEPROM
    ext_eeprom_write(eeprom_end_addr + 5, current_speed);

    //increment eeprom address
    eeprom_end_addr = eeprom_end_addr + 6;
}