/**
 * src/pins.h
 * 
 * Pin bindings for the Mega shield
 * 
 * @author Connor Henley, @thatging3rkid
 */
#ifndef _asc_pins
#define _asc_pins

#include "game.h"

#include <stdint.h>

// Scoring sensors
#define SCORE_SENSOR0 A11
#define SCORE_SENSOR1 A12
#define SCORE_SENSOR2 A13
#define SCORE_SENSOR3 A14

// LED headers
#define LED_HEADER_R 6
#define LED_HEADER_G 5
#define LED_HEADER_B 7

// Debugging LED
#define DLED_R 8
#define DLED_G 10
#define DLED_B 9

// Fan control pins
#define FAN_PWM0 11
#define FAN_PWM1 12
#define FAN_PWM2 13
#define FAN_PWM3 14

const uint8_t FAN_PWM_PINS[] = {FAN_PWM0, FAN_PWM1, FAN_PWM2, FAN_PWM3};

#endif

