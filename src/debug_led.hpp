/**
 * src/debug_led.hpp
 * 
 * Code for configuring the RGB debugging LED on the ASC
 * 
 * @author Connor Henley, @thatging3rkid
 */
#ifndef _debug_led_asc
#define _debug_led_asc

#include <stdint.h>

/**
 * Initialize the debugging LED
 */
void init_led();

/**
 * Set the LED to a color
 * 
 * @param r red value
 * @param g green value
 * @param b blue value
 */
void set_led(uint8_t r, uint8_t g, uint8_t b);

/**
 * Set the LED to a color for a specified amount of time (locking)
 * 
 * @param r red value
 * @param g green value
 * @param b blue value
 * @param time how long the led should be kept on (before returning to it's previous color), in milliseconds
 */
void set_led(uint8_t r, uint8_t g, uint8_t b, uint64_t time);

#endif

