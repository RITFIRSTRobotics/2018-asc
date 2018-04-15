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
#include "debug_led.hpp"
#include "led_strip.hpp"
#include "usbserial.hpp"
#include "fan_control.hpp"
#include "usbser_constants.hpp"

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define SERIAL_BUFFER_SIZE 128

static AllianceColor _color;

void setup() {
  // Initialize serial connections
  init_usbser();
  
  // Initialize pins
  init_scoring();
  init_led();
  init_fans();
  init_led_strip();

  // Wait for serial to startup
  while (!Serial) {;;}

  // Before going to loop(), the ASC needs to be initalized
  uint8_t blink_led_count = 0;
  bool blink_led_state = false;
  unsigned long past_millis = millis();
  while (true) {
    // See if the LED needs to be blinked
    if (blink_led_count != 0) {
      // See if 750ms has gone by
      if (abs(millis() - past_millis) >= 750) {
        past_millis = millis(); // record new time
        blink_led_count -= 1; // decrease the counter
        analogWrite(DLED_R, 0); // tell other colors to turn off
        analogWrite(DLED_B, 0);
        // Check and see if this is the last time it will be called
        if (blink_led_count == 0) { 
          analogWrite(DLED_G, 0); 
        } else {
          // Turn the LED on or off
          analogWrite(DLED_G, blink_led_state? 255 : 0);
          blink_led_state = !blink_led_state;
        }
      }
    }
    
    // See if Serial can be read yet
    if (Serial.available() > 0) {
      // Read Serial into a buffer
      char buffer[64];
      size_t j = 0;
      char in;
      while ((in = Serial.read()) != -1) {
        buffer[j] = in;
        j += 1;
        if (in == '\n' || j > 64) {
          break;
        }
      }

      // Check and see if the data is right
      if (strlen(buffer) > 3 && buffer[0] == INIT_MESSAGE[0] && buffer[1] == INIT_MESSAGE[1]) {
        // Check alliance color
        if (buffer[3] == 'r' || buffer[3] == 'R') {
          _color = RED;
          set_led(255, 0, 0);
          Serial.write(INIT_RESPONSE);
          Serial.write(NEWLINE);
          break;
        }
        if (buffer[3] == 'b' || buffer[3] == 'B') {
          _color = BLUE;
          set_led(0, 0, 255);
          Serial.write(INIT_RESPONSE);
          Serial.write(NEWLINE);
          break;
        }        
      }

      // See if a blink command has been sent
      if (strlen(buffer) > 3 && buffer[0] == BLINK_MESSAGE[0] && buffer[1] == BLINK_MESSAGE[1]) {
        blink_led_count += 9;
      }
    } else {
      delay(20);
    }
  }

  // Initalize i2c at the end (allow for the controllers to boot fully before making an address list)
  init_i2c();
}

void loop() {
  // See if there is incoming data
  if (Serial.available() > 0) {
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
    else if (strlen(buffer) > 8 && buffer[0] == LED_STRIP_SOLID[0]) {
      char location;
      uint32_t num, strip_r, strip_g, strip_b = 0;
      if (buffer[1] == LED_STRIP_SOLID[1]) {
        // Need to parse the data in using sscanf
        sscanf(buffer, LED_STRIP_SOLID, &location, &strip_r, &strip_g, &strip_b);
        set_led_strip_solid(location, strip_r, strip_g, strip_b);
      } else if (buffer[1] == LED_STRIP_WAVE[1]) {
        // Need to parse the data in using sscanf
        sscanf(buffer, LED_STRIP_WAVE, &location, &strip_r, &strip_g, &strip_b)
        set_led_strip_wave(location, strip_r, strip_g, strip_b);
      } else if (buffer[1] == LED_STRIP_NUM[1]) {
        // Need to parse the data in using sscanf
        sscanf(buffer, LED_STRIP_NUM, &location, &num, &strip_r, &strip_g, &strip_b);
        set_led_strip_part(location, num, strip_r, strip_g, strip_b);        
      } else if (buffer[1] == LED_STRIP_ONE[1]) {
        // Need to parse the data in using sscanf
        sscanf(buffer, LED_STRIP_ONE, &num, &strip_r, &strip_g, &strip_b);
        set_led_strip_one(num, strip_r, strip_g, strip_b);        
      }
    }
    // See if a fan command has been sent
    else if (strlen(buffer) > 3 && buffer[0] == BALL_RETURN_CONTROL[0] && buffer[1] == BALL_RETURN_CONTROL[1]) {
      // Parse in the data
      uint8_t fan_num, motor_val = 0;
      sscanf(buffer, BALL_RETURN_CONTROL, &fan_num, &motor_val);

      // Once parsed, send it
      set_ball_fan(fan_num, motor_val);
    }
  }
  
  // Normal handling
  // Start by getting controller data sent and write it to Serial
  process_i2c(&send_usbser);
  
  // Next, handle scoring
  process_scoring(&send_usbser);
  delay(50); // might need to cut this down more
}

