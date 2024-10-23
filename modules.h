/* 
 * Description : 
 * A12- Implement a 8 field password access to screen
 * 
 * Submission Date : 03/10/2024
 *
 * File  : modules.h
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

#ifndef MODULES_H
#define	MODULES_H


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

#endif	/* MODULES_H */

