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
#include "led_strip.hpp"
#include "debug_led.hpp"
#include "usbserial.hpp"
#include "fan_control.hpp"
#include "usbser_constants.hpp"

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

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

      // Check and see if the data is right
      char c1;
      if (sscanf(buffer, INIT_MESSAGE, &c1) == 1) {
        // Check alliance color
        if (tolower(c1) == 'r') {
          _color = RED;
          set_led(255, 0, 0);
          memset(buffer, 0, SERIAL_BUFFER_SIZE);
          sprintf(buffer, INIT_RESPONSE, 0);
          Serial.write(buffer);
          Serial.write(NEWLINE);
          break;
        } else if (tolower(c1) == 'b') {
          _color = BLUE;
          set_led(0, 0, 255);
          memset(buffer, 0, SERIAL_BUFFER_SIZE);
          sprintf(buffer, INIT_RESPONSE, 0);
          Serial.write(buffer);
          Serial.write(NEWLINE);
          break;
        }
      }

      // See if a blink command has been sent
      int i1;
      if (sscanf(buffer, BLINK_MESSAGE, &i1) == 1) {
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
    check_usbser();
  }
  
  // Normal handling
  // Start by getting controller data sent and write it to Serial
  process_i2c(&send_usbser);
  
  // Next, handle scoring
  process_scoring(&send_usbser);
  delay(25); // might need to cut this down more
}

