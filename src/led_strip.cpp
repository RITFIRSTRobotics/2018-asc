/**
 * src/debug_led.cpp
 * 
 * Code for configuring the RGB LED strip powered by the ASC
 * 
 * @author Connor Henley, @thatging3rkid
 */
#include "pins.h"

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_NUM 40

static Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, LED_STRIP_CONTROL, NEO_GRB + NEO_KHZ800);

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
      delay(20);
      strip.show();
    }
  }
  // Case for far LEDs
  if (location == 'f') {
    for (int i = LED_NUM / 2; i < LED_NUM; i += 1) {
      strip.setPixelColor(i, r, g, b);
      delay(20);
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

