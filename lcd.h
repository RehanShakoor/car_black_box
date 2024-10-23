/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : lcd.h
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//lcd.h contains all function declarations and macros related to LCD

#ifndef LCD_H
#define	LCD_H

//include required headers
#include "main.h"

//define LCD data port
#define LCD_DATA_DDR TRISD
#define LCD_DATA_REG PORTD

//define LCD command port
#define LCD_CMD_DDR TRISE
#define LCD_CMD_REG PORTE

//define LCD command pins
#define LCD_EN_PIN BIT1
#define LCD_RS_PIN BIT2


//function declaration to give enable pulse
void lcd_enable_pulse(void);

//function declaration to send LCD data
void lcd_data_send(uint8_t lcd_data);

//function declaration to send LCD command
void lcd_cmd_send(uint8_t lcd_cmd);

//function declaration to initialize LCD
void lcd_init(void);

//function declaration to send string to LCD
void lcd_string_send(char *str);


#endif	/* LCD_H */

