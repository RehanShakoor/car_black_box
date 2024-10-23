/* 
 * Project (Microcontroller): 
 * Car Black Box Project
 * 
 * Submission Date : 03/10/2024
 *
 * File  : i2c.h
 * Author: Rehan Shakoor
 * Batch : 24004
 * 
 */

//This file contains declarations and macros related to I2C

#ifndef I2C_H
#define	I2C_H

//define I2C time delay for write operation
#define I2C_DELAY 10

//function declaration to initialize I2C
void i2c_init(void);

//function declaration to generate i2c start/stop/restart condition
//0,1,2 are for start/restart/stop respectively
void i2c_control(uint8_t ctrl);

//function declaration to write i2c data
void i2c_write(uint8_t data);

//function declaration to read i2c data and send ACK/NACK
uint8_t i2c_read(uint8_t ack);


#endif	/* I2C_H */

