/**
 * src/serial/i2c.cpp
 * 
 * Code for I2C (I squared C) communications between the ASC and controller
 * 
 * @author Connor Henley, @thatging3rkid
 */
#include "i2c_constants.hpp"

#include "debug_led.hpp"

#include <Wire.h>
#include <limits.h>
#include <string.h>
#include <Arduino.h>

static uint8_t addresses[] = {0x10, 0x11, 0x12}; // should move this to #defines

/**
 * @inherit-doc
 */
void init_i2c() {
  Wire.begin(); // master does not need an address
  Wire.setClock(400000L); // go fast
}

/**
 * @inherit-doc
 */
void process_i2c(void (*swrite)(char *)) {
  for (int i = 0; i < 3; i += 1) {
    // Make sure the address is valid first
    if (addresses[i] == UCHAR_MAX) {
      continue;
    }
    
    // For each controller, request a response, then foward it to the FMS
    Wire.requestFrom(addresses[i], (uint8_t) I2CDATA_BUFFER_LEN); // have to cast the constant to make it happy

    // Make a buffer to fill
    char buffer[I2CDATA_BUFFER_LEN];
    memset(buffer, 0, I2CDATA_BUFFER_LEN); // clear out the buffer before starting
    int j = 0;
    
    // Read in data (into the buffer)
    while (Wire.available()) {
      buffer[j] = Wire.read();
      j += 1;
    }

    // Use the function pointer to write to wherever
    (*swrite)(buffer);
  }
}

