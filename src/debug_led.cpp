/**
 * src/debug_led.cpp
 * 
 * Code for configuring the RGB debugging LED on the ASC
 * 
 * @author Connor Henley, @thatging3rkid
 */
#include "pins.h"

#include <Arduino.h>

// Make some variables to store the current LED value
static uint8_t prev_r = 0;
static uint8_t prev_g = 0;
static uint8_t prev_b = 0;

/**
 * @inherit-doc
 */
void init_led() {
  pinMode(DLED_R, OUTPUT);
  pinMode(DLED_G, OUTPUT);
  pinMode(DLED_B, OUTPUT);
}

/**
 * Internal method used to set the LED color
 */
static void _set_led(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(DLED_R, r);
  analogWrite(DLED_G, g);
  analogWrite(DLED_B, b);
}

/**
 * @inherit-doc
 */
void set_led(uint8_t r, uint8_t g, uint8_t b) {
  prev_r = r;
  prev_g = g;
  prev_b = b;
  _set_led(r, g, b);
}

/**
 * @inherit-doc
 */
void set_led(uint8_t r, uint8_t g, uint8_t b, uint64_t time) {
  // Save the time and set the LED
  _set_led(r, g, b);
  delay(time);
  _set_led(prev_r, prev_g, prev_b);
}

