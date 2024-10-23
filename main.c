/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : main.c
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//include required headers
#include "main.h"

//disable watchdog timer
#pragma config WDTE = OFF
//select high speed external crystal as clock source
#pragma config FOSC = HS

/*
 * RB4 and RB5 are used to gear up and down respectively
 * RB0 is used for collision
 * long press RB2 (UP) or RB1 (DOWN) to enter password mode
 * RB2 (UP) and RB1 (DOWN) are used to enter password
 * RB2 (UP) and RB1 (DOWN) are used to navigate menu
 */


void main(void) 
{    
    //call function to initialize LCD
    lcd_init();
    
    //call function to initialize ADC
    adc_init();
    
    //call function to initialize I2C
    i2c_init();
    
    //call function to set rtc value
    rtc_set_time(0 ,0 ,0);
    
    //call function to configure switch ports
    sw_init();

    //call function to initialize timer1
    //timer1 is used to check user block delay
    timer1_init();

    //call function to initialize timer1 interrupt
    timer1_interrupt_init();

    //call function to initialize UART transmit
    uart_tx_init();
    
    
    //iterate in infinite loop and print different modes
    while(1)
    {
        //if current mode is operational mode
        if(current_mode == OPR_MDOE_INDEX)
        {
            //call function to show operational mode
            operation_mode();
        }
        else if(current_mode == PWD_MODE_INDEX)
        {
            //call function to show password mode
            password_mode();
        }
        else if(current_mode == MENU_MODE_INDEX)
        {
            //call function to show menu mode
            menu_mode();
        }
    }
    
    return;
}
