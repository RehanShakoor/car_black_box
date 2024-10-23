/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : common.c
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//This file contains common function declaration and macros

//include required headers
#include "main.h"

#ifndef COMMON_H
#define	COMMON_H

//define switch port
#define SW_DDR TRISB
#define SW_REG PORTB

//define switch pins (with interrupt on change)
#define GEAR_UP_PIN   BIT4
#define GEAR_DOWN_PIN BIT5
#define COLL_PIN      BIT0

//define user control switch pins
#define UP_PIN   BIT2
#define DOWN_PIN BIT1

//define password size
#define PWD_SIZE 5

//define number of attempts allowed to enter password
#define PWD_ATTEMPTS 3

//define switch de-bounce time
#define SW_DEBOUNCE 10

//define swtich long press time in ms
#define SW_LONG_TIME 1000

//define special message display duration
#define MSG_DELAY 2000

//define variable to store count of gears
#define GEAR_COUNT 6


//function declaration to configure switch ports
void sw_init(void);

//function declaration to change gear value 
void change_gear(uint8_t sw_read);

//function declaration to read UP/DOWN switch
//this function also gives switch pressed time
//it is non-blocking function
uint8_t read_switch(uint16_t *milliseconds_count);

//function definition to read password from user
void read_password(char *user_entered_pwd);

//function declaration to display error message on LCD
void lcd_display_err_msg(uint8_t err_count);

//function declaration to display user blocked message
//and display how much time remains to unblock
void lcd_display_blocked_msg(uint8_t time_remain);

//function declaration to change menu index
void change_menu_index(uint8_t *current_menu_index, uint8_t read_val);

//function declaration to display menu option according to current index
void display_menu_option(uint8_t current_menu_index);

//function declaration to enter required menu option
void goto_menu_option(uint8_t current_menu_index);

#endif	/* COMMON_H */

