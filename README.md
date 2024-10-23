# car_block_box

About project :  
Simulated a black box device for a car, using PIC16F877A an 8-bit microcontroller, with bare-metal coding in C.  
An RTC is used to get time via I2C protocol and a potentiometer is used to change speed value using ADC module.  
On gear change by an interrupt switch, current time, speed and gear value are stored in external EEPROM via I2C.  
User can login into device using a password, and can view logs on LCD, change password, set time, clear logs.  
User can download logs, i.e. data from external EEPROM is read and send to serial terminal via UART.  

Project report filename is  'project_report.odt'  
All source code and header files are uploaded in this repository, we need to use MPLAB X IDE is compile code.
