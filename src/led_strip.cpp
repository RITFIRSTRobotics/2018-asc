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

#define LED_NUM 15

static Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, LED_STRIP_CONTROL, NEO_GRB + NEO_KHZ800);

/**
 * Delay the CPU while doing stuff in the background (sending controller data and scoring)
 * 
 * @param ms milliseconds to wait 
 * @note the background stuff is done, then the CPU waits for the time to elapse, because of this
 *       this is not a suitable method for really short or long delays
 */
static void delay_alt(long ms) {
  uint64_t goaltime = millis() + ms;
  
  // Start by getting controller data sent and write it to Serial
  process_i2c(&send_usbser);
  
  // Next, handle scoring
  process_scoring(&send_usbser);

  while (goaltime > millis() && millis() > 100) {
    delay(10);
  }
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
 * @inherit-doc
 */
void set_led_strip_solid(char location, uint8_t r, uint8_t g, uint8_t b) {
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
 * @inherit-doc
 */
void set_led_strip_wave(char location, uint8_t r, uint8_t g, uint8_t b) {
  // Case for close LEDs
  if (location == 'c') {
    for (int i = 0; i < LED_NUM / 2; i += 1) {
      strip.setPixelColor(i, r, g, b);
      delay_alt(50);
      strip.show();
    }
  }
  // Case for far LEDs
  if (location == 'f') {
    for (int i = LED_NUM / 2; i < LED_NUM; i += 1) {
      strip.setPixelColor(i, r, g, b);
      delay_alt(50);
      strip.show();
    }
  }
}

/**
 * @inherit-doc
 */
void set_led_strip_part(char location, uint8_t num, uint8_t r, uint8_t g, uint8_t b) {
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
 * @inherit-doc
 */
void set_led_strip_one(uint8_t num, uint8_t r, uint8_t g, uint8_t b) {
  strip.setPixelColor(num, r, g, b);
  strip.show();  
}


/**
 * @inherit-doc
 */
void check_serial_led(char* buffer) {
  char location;
  uint16_t num, strip_r, strip_g, strip_b = 0;
  if (buffer[1] == LED_STRIP_SOLID[1]) {
    // Need to parse the data in using sscanf
    sscanf(buffer, LED_STRIP_SOLID, &location, &strip_r, &strip_g, &strip_b);
    set_led_strip_solid(location, strip_r, strip_g, strip_b);
  } else if (buffer[1] == LED_STRIP_WAVE[1]) {
    // Need to parse the data in using sscanf
    sscanf(buffer, LED_STRIP_WAVE, &location, &strip_r, &strip_g, &strip_b);
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

