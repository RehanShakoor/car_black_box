/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : modes.h
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//This file contains function declarations and macros related to different modes

#ifndef MODES_H
#define	MODES_H

//define operational mode index
#define OPR_MDOE_INDEX 0
//define password mode index
#define PWD_MODE_INDEX 1
//define menu mode index
#define MENU_MODE_INDEX 2

//define user blocked time in seconds
#define BLOCK_TIME 120

//define varible to store menu count
#define MENU_COUNT 5

//define view log option index
#define VIEW_LOG_INDEX 0
//define set time mode option index
#define SET_TIME_INDEX 1
//define download log option index
#define DOWN_LOG_INDEX 2
//define clear log option index
#define CLEAR_LOG_INDEX 3
//define change password option index
#define CHANGE_PWD_INDEX 4


//function declaration for operation mode
void operation_mode(void);

//function declaration to show password mode
void password_mode(void);

//function declaration to show menu mode
void menu_mode(void);

//function declaration to set RTC time
//new set time is stored in internal EEPROM
void set_time_mode(void);

//function declaration to change password and store it in internal EEPROM
void change_pwd_mode(void);

//function declaration for view log mode
void view_log_mode(void);

//function declaration to download log mode
void download_log_mode(void);

#endif	/* MODES_H */

