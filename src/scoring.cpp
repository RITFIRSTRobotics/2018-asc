/**
 * src/scoring.cpp
 * 
 * Code for the scoring subsystem of the ASC
 * 
 * @author Connor Henley, @thatging3rkid
 */
#include "pins.h"
#include "scoring.hpp"

#include "usbser_constants.hpp"

#include <stdio.h>
#include <Arduino.h>

// Have some variables store the threshold needed for a goal to be detected
static uint16_t _goal0_threshold = DEFAULT_THRESHOLD;
static uint16_t _goal1_threshold = DEFAULT_THRESHOLD;
static uint16_t _goal2_threshold = DEFAULT_THRESHOLD;
static uint16_t _goal3_threshold = DEFAULT_THRESHOLD;

/**
 * @inherit-doc
 */
void init_scoring() {
  pinMode(SCORE_SENSOR0, INPUT);
  pinMode(SCORE_SENSOR1, INPUT);
  pinMode(SCORE_SENSOR2, INPUT);
  pinMode(SCORE_SENSOR3, INPUT);
}

/**
 * @inherit-doc
 */
void update_threshold(uint8_t goal_num) {
  // Figures out the goal number, then makes the threshold a little higher than it
  switch(goal_num) {
    case 0:
      _goal0_threshold = THRESHOLD_MULTIPLIER * analogRead(SCORE_SENSOR0);
      break;
    case 1:
      _goal1_threshold = THRESHOLD_MULTIPLIER * analogRead(SCORE_SENSOR1);
      break;
    case 2:
      _goal2_threshold = THRESHOLD_MULTIPLIER * analogRead(SCORE_SENSOR2);
      break;
    case 3:
      _goal3_threshold = THRESHOLD_MULTIPLIER * analogRead(SCORE_SENSOR3);
      break;
  }
}

/**
 * @inherit-doc
 */
void process_scoring(void (*swrite)(char *)) {
  // Check each goal, can't really loop in case the pins aren't in order (unknown at the time of writing)
  char buffer[8];
  if (analogRead(SCORE_SENSOR0) <= _goal0_threshold) {
    // Print data to a buffer, then use the given function
    sprintf(buffer, SCORE_DATA, 0);
    (*swrite)(buffer);
  }
  if (analogRead(SCORE_SENSOR1) <= _goal1_threshold) {
    // Print data to a buffer, then use the given function
    sprintf(buffer, SCORE_DATA, 1);
    (*swrite)(buffer);
  } 
  if (analogRead(SCORE_SENSOR2) <= _goal2_threshold) {
    // Print data to a buffer, then use the given function
    sprintf(buffer, SCORE_DATA, 2);
    (*swrite)(buffer);
  }
  if (analogRead(SCORE_SENSOR3) <= _goal3_threshold) {
    // Print data to a buffer, then use the given function
    sprintf(buffer, SCORE_DATA, 3);
    (*swrite)(buffer);
  }
}

