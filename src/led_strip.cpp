/**
 * src/debug_led.cpp
 * 
 * Code for configuring the RGB LED strip powered by the ASC
 * 
 * @author Connor Henley, @thatging3rkid
 */
#include "pins.h"
#include "i2c.hpp"
#include "scoring.hpp"
#include "usbserial.hpp"
#include "usbser_constants.hpp"

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_NUM 106
#define LED_INCREMENT 5 // changing this value may cause overflow/underflow
#define LED_DELAY 80 // same as old FMS code, may need to be adjusted

static Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, LED_STRIP_CONTROL, NEO_GRB + NEO_KHZ800);
static uint64_t goaltime = 0;
static uint8_t remaining_cycles = 0;

/**
 * Delay the CPU while doing stuff in the background (sending controller data and scoring)
 * 
 * @param ms milliseconds to wait 
 * @note the background stuff is done, then the CPU waits for the time to elapse, because of this
 *       this is not a suitable method for really short or long delays
 */
static void delay_alt(uint64_t ms) {
  goaltime = millis() + ms;

  while (goaltime > millis() && remaining_cycles != 0) {
    // See if there is incoming data
    if (Serial.available() > 0) {
      check_usbser();
    }
        
    // Start by getting controller data sent and write it to Serial
    process_i2c(&send_usbser);
    
    // Next, handle scoring
    process_scoring(&send_usbser);

    // Sleep a little
    delay(10);
  }

  goaltime = 0;
}

/**
 * @inherit-doc
 */
void init_led_strip() {
  strip.begin();
  strip.setBrightness(80);
  strip.show();
}

/**
 * Set the LED strip to a color
 * 
 * @param location 'c' for close, 'f' for far
 * @param r red value
 * @param g green value
 * @param b blue value
 */
static void set_led_strip_solid(char location, uint8_t r, uint8_t g, uint8_t b) {
  // Case for close LEDs
  if (location == 'c') {
    for (int i = 0; i < LED_NUM / 2; i += 1) {
      strip.setPixelColor(i, r, g, b);
    }
    strip.show();
  }
  // Case for far LEDs
  if (location == 'f') {
    for (int i = LED_NUM / 2; i < LED_NUM; i += 1) {
      strip.setPixelColor(i, r, g, b);
    }
    strip.show();
  }
}

/**
 * Set the LED strip to a color in a wave configuration
 * 
 * @param location 'c' for close, 'f' for far
 * @param r red value
 * @param g green value
 * @param b blue value
 */
static void set_led_strip_wave(char location, uint8_t r, uint8_t g, uint8_t b) {
  // Case for close LEDs
  remaining_cycles = LED_NUM / 2;
  if (location == 'c') {
    while (true) {
      // Set the LED
      strip.setPixelColor((LED_NUM / 2) - remaining_cycles, r, g, b);
      delay_alt(50);
      strip.show();

      // Move to the next LED or stop looping
      if (remaining_cycles == 0) {
        break;
      } else {
        remaining_cycles -= 1;
      }
    }
  }
  // Case for far LEDs
  if (location == 'f') {
    while (true) {
      // Set the LED
      strip.setPixelColor(LED_NUM - remaining_cycles, r, g, b);
      delay_alt(50);
      strip.show();

      // Move to the next LED or stop looping
      if (remaining_cycles == 0) {
        break;
      } else {
        remaining_cycles -= 1;
      }
    }
  }
}

/**
 * Set part of the LED strip to a color
 * 
 * @param location 'c' for close, 'f' for far
 * @param num the number of LEDs to change
 * @param r red value
 * @param g green value
 * @param b blue value
 */
static void set_led_strip_part(char location, uint8_t num, uint8_t r, uint8_t g, uint8_t b) {
  // Case for close LEDs
  if (location == 'c') {
    for (int i = 0; i < num; i += 1) {
      strip.setPixelColor(i, r, g, b);
    }
    strip.show();
  }
  // Case for far LEDs
  if (location == 'f') {
    for (int i = LED_NUM; i > LED_NUM - num; i -= 1) {
      strip.setPixelColor(i, r, g, b);
    }
    strip.show();
  }
}

/**
 * Set part of the LED strip to a color
 * 
 * @param num the number of the LED to change
 * @param r red value
 * @param g green value
 * @param b blue value
 */
static void set_led_strip_one(uint8_t num, uint8_t r, uint8_t g, uint8_t b) {
  strip.setPixelColor(num, r, g, b);
  strip.show();  
}

/**
 * Return the status of the automatically generated LEDs
 *
 * @param r the current red value
 * @param g the current green value
 * @param b the current blue value
 */
static void inline autowave_return(uint8_t r, uint8_t g, uint8_t b) {
  // Generate the return string
  char buffer[SERIAL_BUFFER_SIZE];
  memset(buffer, 0, SERIAL_BUFFER_SIZE);
  sprintf(buffer, LED_STRIP_AUTOWAVE_RESULTS, r, g, b);

  send_usbser(buffer);
  goaltime = 0;
}

/**
 * Automatically generate colors for the LED strip
 *
 * @param r initial red value
 * @param g initial green value
 * @param b initial blue value
 */
static void autowave_run(uint8_t r, uint8_t g, uint8_t b) {
  if (goaltime != 0) {
    // Another autowave start command was sent, just ignore it
    return;
  }

  // Loop through the colors
  remaining_cycles = LED_NUM;
  while (true) {
    // Generate a new color
    if (r == 255 && b == 0 && g < 255) {
      g += LED_INCREMENT;
    } else if (g == 255 && b == 0 && r > 0) {
      r -= LED_INCREMENT;
    } else if (r == 0 && g == 255 && b < 255) {
      b += LED_INCREMENT;
    } else if (r == 0 && b == 255 && g > 0) {
      g -= LED_INCREMENT;    
    } else if (g == 0 && b == 255 && r < 255) {
      r += LED_INCREMENT;
    } else if (r == 255 && g == 0 && b > 0) {
      b -= LED_INCREMENT;
    }

    // Set the pixel and wait (while still processing input)
    strip.setPixelColor(LED_NUM - remaining_cycles, r, g, b);
    strip.show();
    delay_alt(LED_DELAY);

    // See if we should continue
    if (remaining_cycles == 0) {
      break;
    } else {
      remaining_cycles -= 1;
    }
  }

  autowave_return(r, g, b);
}

/**
 * @inherit-doc
 */
void check_serial_led(char* buffer) {
  char location;
  uint16_t num, strip_r, strip_g, strip_b = 0;

  // Find the matching LED command
  if (buffer[0] == LED_STRIP_SOLID[0]) {
    if (buffer[1] == LED_STRIP_SOLID[1]) {
      sscanf(buffer, LED_STRIP_SOLID, &location, &strip_r, &strip_g, &strip_b);
      set_led_strip_solid(location, strip_r, strip_g, strip_b);
    } else if (buffer[1] == LED_STRIP_WAVE[1]) {
      sscanf(buffer, LED_STRIP_WAVE, &location, &strip_r, &strip_g, &strip_b);
      set_led_strip_wave(location, strip_r, strip_g, strip_b);
    } else if (buffer[1] == LED_STRIP_NUM[1]) {
      sscanf(buffer, LED_STRIP_NUM, &location, &num, &strip_r, &strip_g, &strip_b);
      set_led_strip_part(location, num, strip_r, strip_g, strip_b);        
    } else if (buffer[1] == LED_STRIP_ONE[1]) {
      sscanf(buffer, LED_STRIP_ONE, &num, &strip_r, &strip_g, &strip_b);
      set_led_strip_one(num, strip_r, strip_g, strip_b);        
    }
  // LED autowave commands
  } else if (buffer[0] == LED_STRIP_AUTOWAVE_START[0]) {
    if (buffer[1] == LED_STRIP_AUTOWAVE_START[1]) {
      sscanf(buffer, LED_STRIP_AUTOWAVE_START, &strip_r, &strip_g, &strip_b);
      autowave_run(strip_r, strip_g, strip_b);
    } else if (buffer[1] == LED_STRIP_AUTOWAVE_STOP[1]) {
      sscanf(buffer, LED_STRIP_AUTOWAVE_STOP, &num);
      remaining_cycles = num;
    }
  }
}
