/**
 * src/serial/usbserial.cpp
 * 
 * Code for Serial (serial over USB) interactions with the FMS
 * 
 * @author Connor Henley
 */
#include "usbser_constants.hpp"
#include "usbserial.hpp"

#include <stdlib.h>
#include <Arduino.h>

/**
 * @inherit-doc
 */
void init_usbser() {
  Serial.begin(BAUD_RATE); // should be defined to be super fast
}

/**
 * @inherit-doc
 */
void send_usbser(char * data) {
  // Make sure the string is not empty (and exists)
  if (data == NULL || data[0] == 0) {
    return;
  }
  
  while (true) {
    // See if Serial is available
    if (Serial.availableForWrite()) {
      Serial.print(data);
      Serial.print(NEWLINE);
      break;
    } else {
      delay(10); // Delay to let the Serial data transmit
    }
  }
}


