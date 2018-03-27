/**
 * src/debug_led.cpp
 * 
 * Code for configuring the RGB LED strip powered by the ASC
 * 
 * @author Connor Henley, @thatging3rkid
 */
#include "pins.h"

#include <Arduino.h>

/**
 * @inherit-doc
 */
void init_led_strip() {
  pinMode(LED_HEADER_R, OUTPUT);
  pinMode(LED_HEADER_G, OUTPUT);
  pinMode(LED_HEADER_B, OUTPUT);
}

/**
 * @inherit-doc
 */
void set_led_strip(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(LED_HEADER_R, r);
  analogWrite(LED_HEADER_G, g);
  analogWrite(LED_HEADER_B, b);
}


