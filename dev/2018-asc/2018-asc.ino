/**
 * 2018-asc
 * 
 * Arduino Scoring and Controller subsystem firmware
 * 
 * @author Connor Henley, @thatging3rkid
 */
#include "i2c.hpp"
#include "usbserial.cpp"

void setup() {
  // Initialize serial connections
  init_usbser();
  init_i2c();

  // Initialize pins
}

void loop() {
  
}

