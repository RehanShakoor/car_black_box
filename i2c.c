/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : i2c.c
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//This file contains function definitions related to I2C

//include required headers
#include "main.h"

//function definition to initialize I2C
void i2c_init(void)
{
    //configure SDA (RC4) and SCL (RC3) as input
    TRISC |= 0x18;
    
    //set SSPEN (bit5), to configure i2c pins as serial port pins
    SSPCON |= BIT5;
    
    //SSPM3:SSPM0 (bit3:0) (1000), to select i2c master mode
    SSPCON &= 0xF0;
    SSPCON |= BIT3;
    
    //set SMP (bit7), to disable slew rate control for standard speed
    SSPSTAT |= BIT7;
    
    //set i2c clock speed to 100 kHz
    //assign value to SSPADD accordingly
    SSPADD = 49;
}

//function definition to generate i2c start/stop/restart condition
//0,1,2 are for start/restart/stop respectively
void i2c_control(uint8_t ctrl)
{
    //set bit, to initiate start/stop/restart condition on SDA and SCL
    SSPCON2 |= (1 << ctrl);
    
    //wait for start/stop/restart condition to complete
    while(SSPCON2 & (1 << ctrl));
    
    //clear SSPIF (bit3) flag
    PIR1 &= 0xF7;
}

//function definition to write i2c data
void i2c_write(uint8_t data)
{
    //load data into SSPBUF register
    SSPBUF = data;
    
    //wait till data tranmission is complete
    //SSPIF (bit3) is set on completion
    while(!(PIR1 & BIT3));
    
    //clear SSPIF (bit3) flag
    PIR1 &= 0xF7;
    
    //wait till acknowledge is not received from slave
    //ACKSTAT (bit6) is clear, when acknowledge is received
    while(SSPCON2 & BIT6);
    
    //give some delay so that EEPROM can process this data
    __delay_ms(I2C_DELAY);
}

//function definition to read i2c data and send ACK/NACK
uint8_t i2c_read(uint8_t ack)
{
    //set RCEN (bit3), to enable i2c receive mode
    SSPCON2 |= BIT3;
    
    //wait till data reception is complete
    //SSPIF (bit3) is set on completion
    while(!(PIR1 & BIT3));
    
    //clear SSPIF (bit3) flag
    PIR1 &= 0xF7;
    
    //wait till SSPBUF is not full
    //BF (bit0) is set on receive complete
    while(!(SSPSTAT & BIT0));
    
    //read received data from SSPBUF
    uint8_t data = SSPBUF;
    
    //send ACK (acknowledged) to instruct salve to 
    //continue sending more data from subsequent memory
    
    //or send NACK (not acknowledge) to instruct slave to 
    //stop sending more data, which is to be read

    if(ack)
    {
        //clear ACKDT (bit5), to send ACK
        SSPCON2 &= 0xDF;
        //set ACKEN (bit4), to enable acknowledge sequence
        SSPCON2 |= BIT4;
        //wait till ACK is not send
        //ACKEN (bit4) is set on completion
        while(SSPCON2 & BIT4);        
    }
    else
    {    
        //set ACKDT (bit5), to send NACK
        SSPCON2 |= BIT5;
        //set ACKEN (bit4), to enable acknowledge sequence
        SSPCON2 |= BIT4;
        //wait till NACK is not send
        //ACKEN (bit4) is set on completion
        while(SSPCON2 & BIT4);
    }
    
    //give some delay so that EEPROM can process this data
    __delay_ms(I2C_DELAY);
    
    //return received data
    return data;
}
