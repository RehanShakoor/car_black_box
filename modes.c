/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : modes.c
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//This file contains function declarations related to different modes

//include required headers
#include "main.h"

//define global variable to store current mode
//default mode is operational mode
//0 -> operational mode, 1 -> password mode
//2-> menu mode
uint8_t current_mode = 0;

//global variable to store correct password
char stored_pwd[PWD_SIZE] = {UP_PIN, DOWN_PIN,UP_PIN, DOWN_PIN, '\0'};


//function definition for operation mode
void operation_mode(void)
{    
    //move cursor to the first line of LCD
    lcd_cmd_send(0x80);
    __delay_ms(1);
    
    //print header info on first line of LCD
    lcd_string_send(" TIME     E  SP ");
    
    //move cursor to the second line of LCD
    lcd_cmd_send(0xC0);
    __delay_ms(1);

    //call function to read and update time from RTC (DS1307)
    rtc_read_time();

    //print second line information
    //display current time
    lcd_string_send(" ");
    lcd_string_send(current_time);
    
    //display last event
    lcd_string_send(" ");
    lcd_string_send(last_event);
    
    
    //read ADC to get current speed value
    current_speed = (uint8_t) ((adc_read() / 1023.0) * 99);
    //display speed (send ASCII value)
    lcd_string_send(" ");
    lcd_data_send((current_speed / 10) + 48);
    lcd_data_send((current_speed % 10) + 48);


    //variable to store milli-seconds passed
    uint16_t milliseconds_count = 0;
    //call function to read switch 
    uint8_t read_val = read_switch(&milliseconds_count);
    
    //if UP or DOWN switch is pressed
    if(read_val) 
    {
        //if user is blocked
        if(user_blocked) 
        {
            //call function to display user blocked message
            //and display how much time remains to unblock
            lcd_display_blocked_msg(BLOCK_TIME - blocked_time_passed);
        }    
        //if user is not blocked
        else
        {
            //change current mode to password
            current_mode = PWD_MODE_INDEX;
        } 
    } 
}

//function definition to show password mode
void password_mode(void)
{   
    //variable to store user entered password
    char user_entered_pwd[PWD_SIZE];
    
    //variable to store count of error password entered
    uint8_t err_count = 0;
    
    //iterate in infinite loop and read password from user
    while(1)
    {
        //send command to clear LCD
        lcd_cmd_send(0x01);
        __delay_ms(10);

        //print header info on first line of LCD
        lcd_string_send(" ENTER PASSWORD ");

        //call function to read password from user
        read_password(user_entered_pwd);

        //if user entered password is correct
        if(strcmp(user_entered_pwd, stored_pwd) == 0)
        {

            //change current mode to menu mode (2)
            current_mode = MENU_MODE_INDEX;

            //then return from current function
            return;
        }
        //if passwords is in-correct
        else
        {
            //increment error count
            err_count = err_count + 1;
            
            //call function to display error message on LCD
            lcd_display_err_msg(err_count);
            
            //if in-correct password is entered for threshold times
            if(err_count == PWD_ATTEMPTS)
            {
                //change current mode to operation mode
                current_mode = OPR_MDOE_INDEX;
                
                //block the user for 2 mins
                user_blocked = 1;

                //start timer for blocked user
                //reset blocked_time_passed to 0
                blocked_time_passed = 0;  
                //reset TMR1 value
                TMR1 = 15536;
                //set TMR1ON (bit0), to enable timer1
                T1CON |= BIT0;

                //send command to clear LCD
                lcd_cmd_send(0x01);
                __delay_ms(10);
                
                //then return from current function
                return;
            }
        }
    }
}

//function definition to show menu mode
void menu_mode(void)
{    
    //variable to store current menu index
    uint8_t current_menu_index = 0;

    //iterate in infinite loop and read user switch UP/DOWN
    while(1)
    {
        //variable to store milli-seconds passed
        uint16_t milliseconds_count = 0;

        //call function to read switch 
        uint8_t read_val = read_switch(&milliseconds_count);

        //if UP or DOWN switch is pressed (pull up configuration)
        if(read_val) 
        {
            //if switch is pressed for more than long press duration
            if(milliseconds_count > SW_LONG_TIME)
            {
                //if UP switch is pressed for long time
                if(read_val == UP_PIN)
                {
                    //send command to clear LCD
                    lcd_cmd_send(0x01);
                    __delay_ms(10);

                    //call function to enter required menu option
                    goto_menu_option(current_menu_index);
                }
                //if DOWN switch is pressed for long time
                else
                {
                    //logout from current account
                    //change current mode to operation mode
                    current_mode = OPR_MDOE_INDEX;

                    //send command to clear LCD
                    lcd_cmd_send(0x01);
                    __delay_ms(10);

                    //print header info
                    lcd_string_send("  LOGGING  OUT  ");

                    //display success message for given time
                    __delay_ms(MSG_DELAY);

                    //return from this mode
                    return;
                }
            }
            //if switch is pressed for less than long press duration
            else
            {
                //call function to change menu index, based on switch pressed
                change_menu_index(&current_menu_index, read_val);
            }
        }

        //call function to display menu option according to current index
        display_menu_option(current_menu_index);
    }
}

//function definition to set RTC time
void set_time_mode(void)
{
    //variables to store time (hour, min, sec)
    uint8_t user_time[3] = {0, 0, 0};

    //variable to store current time index
    uint8_t current_time_index = 0;

    //display time header info on LCD
    lcd_string_send("    HH:MM:SS    ");

    //interate in infinite loop and read time from user
    while(1)
    {
        //variable to store milli-seconds passed
        uint16_t milliseconds_count = 0;

        //call function to read switch 
        uint8_t read_val = read_switch(&milliseconds_count);
    
        //if UP switch is pressed
        if(read_val == UP_PIN)
        {
            //if switch is pressed for long time
            if(milliseconds_count > SW_LONG_TIME)
            {
                //call function to set RTC value
                rtc_set_time(user_time[2], user_time[1], user_time[0]);

                //send command to clear LCD
                lcd_cmd_send(0x01);
                __delay_ms(10);

                //print header info
                lcd_string_send("  NEW TIME SET  ");

                //display success message for given time
                __delay_ms(MSG_DELAY);

                //return from this function
                return;
            }
            //if switch is pressed for short time
            else
            {
                //increment current time index and check its limits
                current_time_index = ((current_time_index + 1) % 3);
            }
        }
        //if DOWN switch is pressed
        else if(read_val == DOWN_PIN)
        {
            //if switch is pressed for long time
            if(milliseconds_count > SW_LONG_TIME)
            {
                //send command to clear LCD
                lcd_cmd_send(0x01);
                __delay_ms(10);

                //print header info
                lcd_string_send("  TIME NOT SET  ");

                //display success message for given time
                __delay_ms(MSG_DELAY);

                //return from this function (logout)
                return;
            }
            //if switch is pressed for short time
            else
            {
                //increment value at current time index 
                user_time[current_time_index] = user_time[current_time_index] + 1;

                //check limits on user entered time
                if((current_time_index == 0) && (user_time[current_time_index] == 24))
                {
                    //correct hour time
                    user_time[current_time_index] = 0;
                }
                else if(user_time[current_time_index] == 60)
                {
                    //correct min and sec time
                    user_time[current_time_index] = 0;
                }
            }
        }

        //place cursor to the 4th index of second line of LCD
        lcd_cmd_send(0xC4);
        __delay_ms(1);

        //display user-entered time (hours)
        lcd_data_send((user_time[0] / 10) + 48);
        lcd_data_send((user_time[0]  % 10) + 48);
        lcd_data_send(':');

        //display user-entered time (minutes)
        lcd_data_send((user_time[1] / 10) + 48);
        lcd_data_send((user_time[1]  % 10) + 48);
        lcd_data_send(':');

        //display user-entered time (seconds)
        lcd_data_send((user_time[2] / 10) + 48);
        lcd_data_send((user_time[2]  % 10) + 48);
    }
}

//function definition to change password
void change_pwd_mode(void)
{
    //variable to store user entered password
    char user_entered_pwd[PWD_SIZE];
    
    //send command to clear LCD
    lcd_cmd_send(0x01);
    __delay_ms(10);

    //print header info on first line of LCD
    lcd_string_send(" ENTER OLD PASS ");

    //call function to read old password from user
    read_password(user_entered_pwd);

    //if old password is correct
    if(strcmp(user_entered_pwd, stored_pwd) == 0)
    {    
        //send command to clear LCD
        lcd_cmd_send(0x01);
        __delay_ms(10);

        //print header info on first line of LCD
        lcd_string_send(" ENTER NEW PASS ");

        //call function to read new password from user
        read_password(user_entered_pwd);

        //variable to store confirm password
        char confirm_pwd[PWD_SIZE];

        //send command to clear LCD
        lcd_cmd_send(0x01);
        __delay_ms(10);

        //print header info on first line of LCD
        lcd_string_send(" VERIFY NEW PWD ");

        //call function to read verify password from user
        read_password(confirm_pwd);

        //if new password is verified
        if(strcmp(confirm_pwd, user_entered_pwd) == 0)
        { 
            //send command to clear LCD
            lcd_cmd_send(0x01);
            __delay_ms(10);

            //print header info on first line of LCD
            lcd_string_send("PASSWORD CHANGED");

            //display success message for given time
            __delay_ms(MSG_DELAY);

            //update stored password with new password
            strcpy(stored_pwd, confirm_pwd);
        }
        else
        {
            //place cursor to the start of first line of LCD
            lcd_cmd_send(0x80);
            __delay_ms(1);

            //display error header message on first line of LCD
            lcd_string_send(" WRONG NEW PASS ");
            
            //move cursor to the start of the second line of LCD
            lcd_cmd_send(0xC0);
            __delay_ms(1);
            
            //display second line info
            lcd_string_send("   TRY  AGAIN   ");
        
            //display error message for given time
            __delay_ms(MSG_DELAY);
        }
    }
    //if old password is in-correct
    else
    {
        //place cursor to the start of first line of LCD
        lcd_cmd_send(0x80);
        __delay_ms(1);

        //display error header message on first line of LCD
        lcd_string_send(" WRONG OLD PASS ");
            
        //move cursor to the start of the second line of LCD
        lcd_cmd_send(0xC0);
        __delay_ms(1);
            
        //display second line info
        lcd_string_send("   TRY  AGAIN   ");
        
        //display error message for given time
        __delay_ms(MSG_DELAY);
    }
}

//function definition for view log mode
void view_log_mode(void)
{
    //if no logs are present
    if(eeprom_end_addr == 0)
    {
        //display no logs on LCD
        //clear the display
        lcd_cmd_send(0x01);
        __delay_ms(10);

        //display header info
        lcd_string_send("   LOGS EMPTY   ");

        //display error message for given time then return
        __delay_ms(MSG_DELAY);
        return;
    }

    //define variable to store index of log
    uint8_t log_index = 0;

    //place cursor to the start of first line of LCD
    lcd_cmd_send(0x80);
    __delay_ms(1);
    
    //print header info on first line of LCD
    lcd_string_send("# TIME     E  SP");

    //define variable to store eeprom memory read address
    uint8_t eeprom_addr = 0;

    //iterate in loop and read all logs
    while(1)
    {
        //call function to read event info from eeprom
        read_event_eeprom(eeprom_addr);

        //move cursor to the second line of LCD
        lcd_cmd_send(0xC0);
        __delay_ms(1);

        //print second line information
        //display log index
        lcd_data_send(log_index + 48);

        //display event time
        lcd_string_send(" ");
        lcd_string_send(event_info.event_time);
    
        //display event signature
        lcd_string_send(" ");
        lcd_string_send(event_info.event_signature);
    
        //display speed (send ASCII value)
        lcd_string_send(" ");
        lcd_data_send((event_info.event_speed / 10) + 48);
        lcd_data_send((event_info.event_speed  % 10) + 48);

            
        //read switch in blocking manner
        while(1)
        {
            //variable to store milli-seconds passed
            uint16_t milliseconds_count = 0;
            //call function to read switch 
            uint8_t read_val = read_switch(&milliseconds_count);
    
            //if DOWN switch is pressed for long time
            if((milliseconds_count > MSG_DELAY) && (read_val == DOWN_PIN)) 
            {
                //return from view log mode
                return;
            }
            //if DOWN switch is pressed for short time
            //only 10 logs can be printed on screen
            else if((read_val == DOWN_PIN) && (log_index != 9))
            {
                //increment eeprom read address and check limits
                if(eeprom_addr != eeprom_end_addr - 6)
                {
                    eeprom_addr = eeprom_addr + 6;
                    //increment log index
                    log_index = log_index + 1;
                }

                //then break from inner loop
                break;
            }
            //if UP switch is pressed for short time
            //only 10 logs can be printed on screen
            else if((read_val == UP_PIN) && (log_index != 0))
            {
                //decrement eeprom read address and check its limits
                if(eeprom_addr)
                {
                    //decrement eeprom memory read address
                    eeprom_addr = eeprom_addr - 6;
                    //decrement log index
                    log_index = log_index - 1;
                }
                    
                //then break from inner loop
                break;
            }
        }
    }

    //read switch in blocking manner
    while(1)
    {
        //variable to store milli-seconds passed
        uint16_t milliseconds_count = 0;
        //call function to read switch 
        uint8_t read_val = read_switch(&milliseconds_count);
    
        //if DOWN switch is pressed for long time
        if((milliseconds_count > MSG_DELAY) && (read_val == DOWN_PIN)) 
        {
            //return from view log mode
            return;
        }
    }
}

//function definition to download log mode
void download_log_mode(void)
{
    //if no logs are present
    if(eeprom_end_addr == 0)
    {
        //display no logs on LCD
        //clear the display
        lcd_cmd_send(0x01);
        __delay_ms(10);

        //display header info
        lcd_string_send("   LOGS EMPTY   ");

        //display error message for given time then return
        __delay_ms(MSG_DELAY);
        return;
    }

    //display progress messgae on LCD
    //clear the display
    lcd_cmd_send(0x01);
    __delay_ms(10);
    //display header info
    lcd_string_send("DOWNLOADING LOGS");

    //define variable to store index of log
    uint8_t log_index = 0;
    //send header info via UART
    uart_tx_string("\n### TIME     E  SP\n");
    //define variable to store eeprom memory read address
    uint8_t eeprom_addr = 0;

    //iterate in loop and read all logs
    while(1)
    {
        //call function to read event info from eeprom
        read_event_eeprom(eeprom_addr);

        //send log index (3 digits) via UART
        uart_tx_data((log_index / 100) + 48);
        uart_tx_data(((log_index / 10) % 10) + 48);
        uart_tx_data((log_index % 10) + 48);

        //sent event time info via UART
        uart_tx_string(" ");
        uart_tx_string(event_info.event_time);
    
        //sent event info via UART
        uart_tx_string(" ");
        uart_tx_string(event_info.event_signature);
    
        //send speed value (2 digits) via UART
        uart_tx_string(" ");
        uart_tx_data((event_info.event_speed / 10) + 48);
        uart_tx_data((event_info.event_speed  % 10) + 48);

        //send newline via UART
        uart_tx_data('\n');

        //increment eeprom read address and check limits
        if(eeprom_addr != eeprom_end_addr - 6)
        {
            eeprom_addr = eeprom_addr + 6;
            //increment log index
            log_index = log_index + 1;
        }
        else
        {
            //display success messgae on LCD
            //clear the display
            lcd_cmd_send(0x01);
            __delay_ms(10);

            //display header info
            lcd_string_send("LOGS  DOWNLOADED");

            //send newline via UART
            uart_tx_data('\n');

            //display error message for given time then return
            __delay_ms(MSG_DELAY);
            return;
        }
    }
}