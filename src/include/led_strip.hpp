/**
 * src/include/led_strip.hpp
 * 
 * Code for configuring the RGB LED strip powered by the ASC
 * 
 * @author Connor Henley, @thatging3rkid
 */
#ifndef _led_strip_asc
#define _led_strip_asc

#include <stdint.h>

/**
 * Initialize the LED strip pins
 */
void init_led_strip();

/**
 * Check Serial over USB for LED commands
 * 
 * @param buffer the data read 
 */
void check_serial_led(char* buffer);

#endif

