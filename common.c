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

//This file contains common function definitions

//include required headers
#include "main.h"

//define global variable to store current time
char current_time[9] = "00:00:00";
//define global variable to store last event
char *last_event = "ON";
//define global variable to store gear values
char* gear_values[GEAR_COUNT] = {"GR", "GN", "G1", "G2", "G3", "G4"};
//define global variable to current speed
uint8_t current_speed = 0;


//function definition to configure switch ports
void sw_init(void)
{
    //configure switch pins as input
    SW_DDR |= (GEAR_UP_PIN | GEAR_DOWN_PIN | COLL_PIN | UP_PIN | DOWN_PIN);

    //clear INTF (bit1) flag
    //clear RBIF (bit0) flag
    //this will prevent initial interrupt trigger
    //read PORTB to clear mismatch, then RBIF will be cleared
    uint8_t temp = SW_REG;
    INTCON &= 0xFC;
    
    //set RBIE (bit3), to enable RB port change interrupt
    //set INTE (bit4), to enable INT external interrupt
    //set GIE (bit7), to enable global interrupt
    INTCON |= 0x98;

    //clear INTEDG (bit6), to enable interrupt on falling edge
    OPTION_REG &= 0xBF;  
}

//function definition to change gear value 
void change_gear(uint8_t sw_read)
{
    //define variable to store current gear value index
    static uint8_t current_gear_index = 1;
    
    //if gear up switch is pressed
    if(sw_read == GEAR_UP_PIN)
    {
        //if last gear value is not reached
        if(current_gear_index != (GEAR_COUNT - 1))
        {
            //increment gear index value
            current_gear_index = current_gear_index + 1;
        }  
    }
    //if gear down switch is pressed
    else
    {
        //if first gear value is not reached
        if(current_gear_index != 0)
        {
            //decrement gear index value
            current_gear_index = current_gear_index - 1;
        }  
    }
    
    //update last event with new gear values
    last_event = *(gear_values + current_gear_index);
}

//function definiition to read UP/DOWN switch
//this function also updates switch pressed time
//it is non-blocking function
uint8_t read_switch(uint16_t *milliseconds_count)
{
    //read two switch pin bits
    uint8_t read_val = (~SW_REG & (UP_PIN | DOWN_PIN));

    //if UP or DOWN switch is pressed (pull up configuration)
    if((read_val == UP_PIN) || (read_val == DOWN_PIN)) 
    {
        //wait for switch press de-bounce
        __delay_ms(SW_DEBOUNCE);
            
        //wait till any switch is pressed
        while((SW_REG & (UP_PIN | DOWN_PIN)) != (UP_PIN | DOWN_PIN))
        {
            //increment milliseconds count
            __delay_ms(1);
            *milliseconds_count = *milliseconds_count + 1;
        }
            
        //wait for switch release de-bounce
        __delay_ms(SW_DEBOUNCE);

        //return read switch value
        return read_val;
    } 

    //return 0, if no switch is pressed
    return 0;
}

//function definition to read password from user
void read_password(char *user_entered_pwd)
{   
    //enable cursor blink
    lcd_cmd_send(0x0F);
    __delay_ms(1);

    //place cursor at 6th index of second line of LCD
    lcd_cmd_send(0xC6);
    __delay_ms(1);
    
    //read 4 character password from user
    uint8_t char_count = 0;
    while(char_count < (PWD_SIZE - 1))
    {
        //variable to store milli-seconds passed
        uint16_t milliseconds_count = 0;

        //call function to read switch 
        uint8_t read_val = read_switch(&milliseconds_count);
    
        //if UP or DOWN switch is pressed
        if(read_val) 
        {
            //store user entered char in password
            user_entered_pwd[char_count] = read_val;
            
            //increment char count
            char_count = char_count + 1;
            
            //display '*' on LCD for each char entered
            lcd_data_send('*');
            __delay_ms(10);
        }  
    }

    //turn off cursor
    lcd_cmd_send(0x0C);
    __delay_ms(1);

    //terminate user entered password with null
    user_entered_pwd[PWD_SIZE - 1] = '\0';
}

//function definition to display error message on LCD
void lcd_display_err_msg(uint8_t err_count)
{            
    //move cursor to the first line of LCD
    lcd_cmd_send(0x80);
    __delay_ms(1);

    //if in-correct password is entered for threshold times
    if(err_count == PWD_ATTEMPTS)
    {
        //display header message on first line of LCD
        lcd_string_send("NO ATTEMPTS LEFT");
            
        //move cursor to the start of the second line of LCD
        lcd_cmd_send(0xC0);
        __delay_ms(1);
            
        //display last message on LCD
        lcd_string_send("TWO MINS BLOCKED");

        //display error message for given time
        __delay_ms(MSG_DELAY);
    }
    //else print error message
    else
    {                   
        //display error header message on first line of LCD
        lcd_string_send(" INCORRECT PWD ");
            
        //move cursor to the start of the second line of LCD
        lcd_cmd_send(0xC0);
        __delay_ms(1);
            
        //display attempts left on LCD
        lcd_data_send((PWD_ATTEMPTS - err_count) + 48);
        lcd_string_send(" ATTEMPTS  LEFT");
        
        //display error message for given time
        __delay_ms(MSG_DELAY);
    }
}

//function definition to display user blocked message
//and display how much time remains to unblock
void lcd_display_blocked_msg(uint8_t time_remain)
{
    //clear the display
    lcd_cmd_send(0x01);
    __delay_ms(10);

    //displat header info
    lcd_string_send("  USER BLOCKED  ");

    //place cursor at start of second line of LCD
    lcd_cmd_send(0xC0);
    __delay_ms(1);

    //display time left to unblock
    lcd_string_send("  WAIT ");

    //send ASCII value of time left
    lcd_data_send(((time_remain / 100) % 10) + '0');
    lcd_data_send(((time_remain / 10) % 10) + '0');
    lcd_data_send((time_remain % 10) + '0');

    lcd_string_send(" SEC  ");

    //display error message for given time
    __delay_ms(MSG_DELAY);
}

//function definition to change menu index
void change_menu_index(uint8_t *current_menu_index, uint8_t read_val)
{
    //if DOWN switch is pressed
    if(read_val == DOWN_PIN)
    {
        //increment current menu index and check limits
        *current_menu_index = *current_menu_index == 4 ? 4 : *current_menu_index + 1; 
    }
    //if down switch is pressed
    else
    {
        //decrement current menu index and check limits
        *current_menu_index = *current_menu_index == 0 ? 0 : *current_menu_index - 1; 
    }
}

//function definition to display menu option according to current index
void display_menu_option(uint8_t current_menu_index)
{
    //define variable to store menu options
    char *menu_options[MENU_COUNT  + 1] = {"VIEW LOG      ", "SET TIME      ", "DOWNLOAD LOG  ", 
         "CLEAR LOG     ", "CHANGE PWD    ", "              "};

    //place cursor to the start of first line of LCD
    lcd_cmd_send(0x80);
    __delay_ms(1);

    //if current menu index is odd
    if(current_menu_index % 2)
    {
        //display menu option on fist line
        lcd_string_send("  ");
        lcd_string_send(menu_options[current_menu_index - 1]);

        //place cursor to the second line of LCD
        lcd_cmd_send(0xC0);
        __delay_ms(1);

        //display arrow on LCD second line 
        lcd_string_send("> ");

        //display menu option on second line
        lcd_string_send(menu_options[current_menu_index]);
    }
    //if current menu index is even
    else
    {
        //display arrow on LCD first line 
        lcd_string_send("> ");

        //display menu option on first line
        lcd_string_send(menu_options[current_menu_index]);

        //place cursor to the second line of LCD
        lcd_cmd_send(0xC0);
        __delay_ms(1);

        //display menu option on second line
        lcd_string_send("  ");
        lcd_string_send(menu_options[current_menu_index + 1]);
    }
}

//function definition to enter required menu option
void goto_menu_option(uint8_t current_menu_index)
{
    //if SET TIME option is selected
    if(current_menu_index == SET_TIME_INDEX)
    {
        //call function to set time mode
        //new set time is stored in internal EEPROM
        set_time_mode();
    }
    //if change password option is selected
    else if(current_menu_index == CHANGE_PWD_INDEX)
    {
        //call function to change password mode
        change_pwd_mode();
    }
    //if view log option is selected
    else if(current_menu_index == VIEW_LOG_INDEX)
    {
        //call function to view log mode
        view_log_mode();
    }
    //if view log option is selected
    else if(current_menu_index == CLEAR_LOG_INDEX)
    {
        //set eeprom end address to zero, to denotes no logs are stored
        eeprom_end_addr = 0;

        //send command to clear LCD
        lcd_cmd_send(0x01);
        __delay_ms(10);

        //print header info
        lcd_string_send("  LOGS CLEARED  ");

        //display success message for given time
        __delay_ms(MSG_DELAY);
    }
    //if download log option is selected
    else if(current_menu_index == DOWN_LOG_INDEX)
    {
        //call function to download log mode
        download_log_mode();
    }
}