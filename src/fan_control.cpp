/**
 * src/fan_control.cpp
 * 
 * Code for configuring fans connected to the ASC
 * 
 * @author Connor Henley, @thatging3rkid
 */
#include "pins.h"

#include <Arduino.h>

/**
 * @inherit-doc
 */
void init_fans() {
  // Loop over all the fan pins
  for (int i = 0; i < (int) sizeof(FAN_PWM_PINS); i += 1) {
    pinMode(FAN_PWM_PINS[i], OUTPUT);
  }
}

/**
 * @inherit-doc
 */
void set_ball_fan(uint8_t fan, uint8_t speed) {
  // Make sure to check for bounds
  if (0 < fan && fan < sizeof(FAN_PWM_PINS)) {
    analogWrite(FAN_PWM_PINS[fan], speed);
  }
}

