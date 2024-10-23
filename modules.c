/* 
 * Description : 
 * A12- Implement a 8 field password access to screen
 * 
 * Submission Date : 03/10/2024
 *
 * File  : modules.c
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//include required headers
#include "main.h"
#include "modules.h"


//function definition to give enable pulse
void lcd_enable_pulse(void)
{
    //turn on LCD enable pin
    LCD_CMD_REG |= LCD_EN_PIN;
    
    //wait for 1 us
    __delay_us(1);
    
    //turn off LCD enable pin
    LCD_CMD_REG &= ~(LCD_EN_PIN);
    
    //wait for 1 us
    __delay_us(1);
}

//function definition to send LCD data
void lcd_data_send(uint8_t lcd_data)
{
    //set RS (register select pin), to select LCD data register
    LCD_CMD_REG |= LCD_RS_PIN;
    
    //clear upper nibble of LCD data register
    LCD_DATA_REG &= 0x0F;
    //send only upper nibble of data to LCD data port (4-bit mode)
    LCD_DATA_REG |= (lcd_data & 0xF0);
    
    //give enable pulse, to send data to LCD
    lcd_enable_pulse();
    
    //wait for 1 MS
    __delay_ms(1);
    
    //clear upper nibble of LCD data register
    LCD_DATA_REG &= 0x0F;
    //send only lower nibble of data to LCD data port (4-bit mode)
    LCD_DATA_REG |= ((lcd_data & 0x0F) << 4);
    
    //give enable pulse, to send data to LCD
    lcd_enable_pulse();
    
    //wait for 1 MS
    __delay_ms(1);
}

//function definition to send LCD command
void lcd_cmd_send(uint8_t lcd_cmd)
{
    //clear RS (register select pin), to select LCD command register
    LCD_CMD_REG &= ~(LCD_RS_PIN);
    
    //clear upper nibble of LCD data register
    LCD_DATA_REG &= 0x0F;
    //send only upper nibble of cmd to LCD data port (4-bit mode)
    LCD_DATA_REG |= (lcd_cmd & 0xF0);
    
    //give enable pulse, to send cmd to LCD
    lcd_enable_pulse();
    
    //wait for 1 MS
    __delay_ms(1);
    
    //clear upper nibble of LCD data register
    LCD_DATA_REG &= 0x0F;
    //send only lower nibble of cmd to LCD data port (4-bit mode)
    LCD_DATA_REG |= (lcd_cmd << 4);
    
    //give enable pulse, to send cmd to LCD
    lcd_enable_pulse();
    
    //wait for 1 MS
    __delay_ms(1);
}

//function definition to initialize LCD
void lcd_init(void)
{
    //configure LCD data and command port as output
    LCD_DATA_DDR = 0x00;
    LCD_CMD_DDR  = 0x00;
    
    //clear all pins of data and command port pins
    LCD_DATA_REG = 0x00;
    LCD_CMD_REG  = 0x00;
    
    //select 4-bit, 2 line, 5x7 dots LCD mode
    lcd_cmd_send(0x02);
    __delay_ms(1);
    lcd_cmd_send(0x28);
    __delay_ms(1);
    
    //entry mode (increment cursor, no shift)
    lcd_cmd_send(0x06);
    __delay_ms(1);
    
    //display on, cursor off
    lcd_cmd_send(0x0C);
    __delay_ms(1);
    
    //clear display
    lcd_cmd_send(0x01);
    __delay_ms(10);
}

//function definition to send string to LCD
void lcd_string_send(char *str)
{
    //iterate over string
    while(*str)
    {
        //send each char and increment 'str' pointer
        lcd_data_send(*str);
        str = str + 1;
    }
}