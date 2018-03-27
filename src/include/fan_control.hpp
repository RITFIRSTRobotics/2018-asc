/**
 * src/include/fan_control.hpp
 * 
 * Code for configuring fans connected to the ASC
 * 
 * @author Connor Henley, @thatging3rkid
 */
#ifndef _fan_control_asc
#define _fan_control_asc

#include <stdint.h>

/**
 * Initialize the fan control pins (and set the fan to be 0)
 */
void init_fans();

/**
 * Set a fan to a speed
 * 
 * @param fan the fan number
 * @param speed the speed of the fan on a scale of 0-255
 */
void set_ball_fan(uint8_t fan, uint8_t speed);

#endif
