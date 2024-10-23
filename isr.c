/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : isr.c
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//This file contains ISR (interrupt service routine)

//include required headers
#include "main.h"

//global variable to check user blocked status
uint8_t user_blocked = 0;
//global variable to store blocked start time
uint8_t blocked_time_passed = 0;

//define ISR
void __interrupt() ISR(void)
{   
    //variable to store timer1 overflow count
    static uint8_t timer1_overflow_count = 0; 

    //check RBIF (bit0), for port b interrupt on change
    //gear change switch
    if(INTCON & BIT0)
    {
        //wait for swith press de-bounce time
        __delay_ms(SW_DEBOUNCE);
        
        //read switch value
        uint8_t sw_read = (~SW_REG & (GEAR_UP_PIN | GEAR_DOWN_PIN));

        //if GEAR UP or DOWN switch is pressed
        if((sw_read == GEAR_UP_PIN) || (sw_read == GEAR_DOWN_PIN))
        {
            //wait till any switch is pressed
            while((SW_REG & (GEAR_UP_PIN | GEAR_DOWN_PIN)) != (GEAR_UP_PIN | GEAR_DOWN_PIN));

            //wait for switch release de-bounce time
            __delay_ms(SW_DEBOUNCE);

            //on switch release again mismatch will generate
            //therefore we must read PORTB to clear this mismatch
            uint8_t temp = SW_REG;

            //call function to change gear 
            change_gear(sw_read);

            //call function to store captured event in eeprom
            //this will store time, event and speed
            store_event_eeprom();  
        }
        
        //clear RBIF (bit0) flag
        INTCON &= 0xFE;
    }
    
    //check INTF (bit1), to check INT interrupt
    //collision or password switch 
    if(INTCON & BIT1)
    {
        //delay for switch press de-bounce
        __delay_ms(SW_DEBOUNCE);
        
        //wait till switch is pressed (pull up configuration)
        while(!(SW_REG & COLL_PIN));
        
        //delay for switch release de-bounce
        __delay_ms(SW_DEBOUNCE);

        //change last event to "CL" on collision
        last_event = "CL";
        
        //call function to store captured event in eeprom
        //this will store time, event and speed
        store_event_eeprom();   
        
        //clear INTF (bit1) flag
        INTCON &= 0xFD;
    }

    //check TMR1IF (bit0), to check timer1 overflow (every 40MS)
    //timer1 is used to check user block delay
    if(PIR1 & BIT0)
    {
        //increment timer1 overflow count
        timer1_overflow_count = timer1_overflow_count + 1;
        
        //25 overflows means 1 seconds have passed
        if(timer1_overflow_count == 25)
        {
            //reset overflow count
            timer1_overflow_count = 0;

            //increment blocked time pass
            blocked_time_passed = blocked_time_passed + 1; 

            //if required block time have passed
            if(blocked_time_passed == BLOCK_TIME)
            {
                //un-block the user
                user_blocked = 0;

                //stop the timer
                //clear TMR1ON (bit0), to disable timer1
                T1CON &= 0xFE;
            }
        }
    
        //reset TMR1 value
        TMR1 = 15536;

        //clear TMR1IF (bit0), to clear timer1 interrupt flag
        PIR1 &= 0xFE;
    }
}