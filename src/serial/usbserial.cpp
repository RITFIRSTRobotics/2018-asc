/**
 * src/serial/usbserial.cpp
 * 
 * Code for Serial (serial over USB) interactions with the FMS
 * 
 * @author Connor Henley
 */
#include "scoring.hpp"
#include "led_strip.hpp"
#include "debug_led.hpp"
#include "usbserial.hpp"
#include "fan_control.hpp"
#include "usbser_constants.hpp"

#include <stdlib.h>
#include <Arduino.h>

static void _reset();

/**
 * @inherit-doc
 */
void init_usbser() {
  Serial.begin(BAUD_RATE); // should be defined to be super fast
}

/**
 * @inherit-doc
 */
void check_usbser() {
  // Make a buffer and read data into it
  char buffer[SERIAL_BUFFER_SIZE];
  size_t j = 0;
  char in;
  while ((in = Serial.read()) != -1) {
    buffer[j] = in;
    j += 1;
    if (in == '\n' || j > SERIAL_BUFFER_SIZE) {
      break;
    }
  }
  
  // See if a calibration has been requested
  if (strlen(buffer) > 3 && buffer[0] == CALIBRATE_MESSAGE[0] && buffer[1] == CALIBRATE_MESSAGE[1]) {
    // Calibrate the specified goal
    update_threshold(buffer[3] - '0');
  }
  // See if an LED strip command had been sent
  else if (strlen(buffer) > 8 && (buffer[0] == LED_STRIP_SOLID[0] || buffer[0] == LED_STRIP_AUTOWAVE_START[0])) {
    check_serial_led(buffer);
   
  // See if a fan command has been sent
  } else if (strlen(buffer) > 3 && buffer[0] == BALL_RETURN_CONTROL[0] && buffer[1] == BALL_RETURN_CONTROL[1]) {
    // Parse in the data
    uint16_t fan_num, motor_val = 0;
    sscanf(buffer, BALL_RETURN_CONTROL, &fan_num, &motor_val);
    // Once parsed, send it
    set_ball_fan(fan_num, motor_val);
  // See if someone tried to reinitialize the ASC
  } else if (strlen(buffer) > 3 && ((buffer[0] == INIT_MESSAGE[0] && buffer[1] == INIT_MESSAGE[1]) || (buffer[0] == BLINK_MESSAGE[0] && buffer[1] == BLINK_MESSAGE[1]))) {
    // Blink the LED
    set_led(0, 0, 0);
    for (int i = 0; i < 3; i += 1) {
      set_led(255, 255, 255, 200);
      set_led(0, 0, 0, 300);
    }
    
    _reset();
  }
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

/**
 * Reset the ASC
 * 
 * @note this will destroy the current state of the ASC
 */
static void _reset() {
  asm volatile("  jmp 0");
}

