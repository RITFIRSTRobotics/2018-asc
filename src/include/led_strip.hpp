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
 * Set the LED strip to a color
 * 
 * @param r red value
 * @param g green value
 * @param b blue value
 */
void set_led_strip(uint8_t r, uint8_t g, uint8_t b);

#endif

