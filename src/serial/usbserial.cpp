/**
 * src/serial/usbserial.cpp
 * 
 * Code for Serial (serial over USB) interactions with the FMS
 * 
 * @author Connor Henley
 */
#include "usbser_constants.hpp"
#include "usbserial.hpp"

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
  while (true) {
    // See if Serial is available
    if (Serial.availableForWrite()) {
      Serial.print(data);
      Serial.print(NEWLINE);
    } else {
      delay(10); // Delay to let the Serial data transmit
    }
  }
}


