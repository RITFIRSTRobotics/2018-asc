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
 * @param location 'c' for close, 'f' for far
 * @param r red value
 * @param g green value
 * @param b blue value
 */
void set_led_strip_solid(char location, uint8_t r, uint8_t g, uint8_t b);

/**
 * Set the LED strip to a color in a wave configuration
 * 
 * @param location 'c' for close, 'f' for far
 * @param r red value
 * @param g green value
 * @param b blue value
 */
void set_led_strip_wave(char location, uint8_t r, uint8_t g, uint8_t b);

/**
 * Set part of the LED strip to a color
 * 
 * @param location 'c' for close, 'f' for far
 * @param num the number of LEDs to change
 * @param r red value
 * @param g green value
 * @param b blue value
 */
void set_led_strip_part(char location, uint8_t num, uint8_t r, uint8_t g, uint8_t b);

#endif

