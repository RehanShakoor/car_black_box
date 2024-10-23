/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : main.h
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//main.h includes all required header, define macros and global variables

#ifndef MAIN_H
#define	MAIN_H

//include in-build headers
#include <xc.h>
#include <stdint.h>
#include <string.h>

//include user defined headers
#include "common.h"
#include "lcd.h"
#include "modes.h"
#include "adc.h"
#include "i2c.h"
#include "rtc.h"
#include "eeprom.h"
#include "timer.h"
#include "uart.h"

//define bits
#define BIT0 (0x01)
#define BIT1 (0x02)
#define BIT2 (0x04)
#define BIT3 (0x08)
#define BIT4 (0x10)
#define BIT5 (0x20)
#define BIT6 (0x40)
#define BIT7 (0x80)

//define system clock frequency (20 MHz)
#define _XTAL_FREQ 20000000

//declare external variables
extern uint8_t current_mode;
extern char current_time[9];
extern char *last_event;
extern char* gear_values[6];
extern uint8_t current_speed;
extern char stored_pwd[PWD_SIZE];
extern uint8_t user_blocked;
extern uint8_t blocked_time_passed;
extern uint8_t eeprom_end_addr;
extern struct event_format event_info;

#endif	/* MAIN_H */

