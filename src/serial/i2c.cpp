/**
 * src/serial/i2c.cpp
 * 
 * Code for I2C (I squared C) communications between the ASC and controller
 * 
 * @author Connor Henley, @thatging3rkid
 */
#include "i2c_constants.hpp"

#include <Wire.h>

/**
 * @inherit-doc
 */
void init_i2c() {
  Wire.begin(); // master does not need an address
}

/**
 * @inherit-doc
 */
void process_i2c(void (*swrite)(char *)) {
  for (int i = 0; i < 3; i += 1) {
    // For each controller, request a response, then foward it to the FMS
    Wire.requestFrom(BASE_ADDR + i, I2CDATA_BUFFER_LEN);

    // Make a buffer to fill
    char buffer[I2CDATA_BUFFER_LEN];
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

