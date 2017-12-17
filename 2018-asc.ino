/**
 * 2018-asc
 * 
 * Arduino Scoring and Controller subsystem firmware
 * 
 * @author Connor Henley, @thatging3rkid
 */
#include "pins.h"
#include "i2c.hpp"
#include "scoring.hpp"
#include "usbserial.hpp"

void setup() {
  // Initialize serial connections
  init_usbser();
  init_i2c();

  // Initialize pins
  init_scoring();
}

void loop() {
  // See if there is incoming data (there should almost never be)
  if (Serial.available() > 0) {
    // handle it?
  } else {
    // Normal handling
    // Start by getting controller data sent and write it to Serial
    process_i2c(&send_usbser);
    
  }
  
}

