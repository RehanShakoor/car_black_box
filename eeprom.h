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

//This file contains function declarations and macros related to eeprom

#ifndef EEPROM_H
#define	EEPROM_H

//include required headers
#include "main.h"

//define external EEPROM read and write address (A2,A1 are grounded)
//A0 is used to select page1 or page2 of external eeprom (only page1 is used)
#define EEPROM_WRITE_ADDRESS 0xA0
#define EEPROM_READ_ADDRESS  0xA1

//define macros for logs
#define LOGS_READ_DONE   0
#define LOGS_READ_FINISH 1

 //define struct to store format of event read from eeprom
struct event_format
{
    //store time, signature and speed at event
    char event_time[9];
    char event_signature[3];
    uint8_t event_speed;
};

//variable to store eeprom end address
uint8_t eeprom_end_addr = 0;


//function declaration to write external EEPROM
void ext_eeprom_write(uint8_t addr, uint8_t data);

//function declaration to read external EEPROM
uint8_t ext_eeprom_read(uint8_t addr);

//function declaration to read event info from eeprom
void read_event_eeprom(uint8_t eeprom_mem_addr_read);

//function declaration to store captured event in eeprom
//this will store time, event and speed
void store_event_eeprom(void);

#endif	/* EEPROM_H */

