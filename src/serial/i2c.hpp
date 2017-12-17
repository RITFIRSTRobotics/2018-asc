/**
 * src/serial/i2c.hpp
 * 
 * Code for I2C (I squared C) communications between the ASC and controller
 * 
 * @author Connor Henley, @thatging3rkid
 */
#ifndef _i2c_com_asc
#define _i2c_com_asc

/**
 * Initialize the ASC as the i2c master
 */
void init_i2c();

/**
 * Get data from all the controllers and write it using the given function pointer
 * 
 * @param swrite a function to write the data (without a newline) to
 */
void process_i2c(void (*swrite)(char *));

#endif
