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
#include <string.h>
#include <Arduino.h>

#define SCORING_BUFFER_LENGTH 16
#define NUM_SENSORS 4
#define NUM_READINGS 3

// Define a struct to save data about each scoring sensor
typedef struct {
  uint8_t pin;
  bool scored;
  uint16_t threshold;
  uint16_t readings[NUM_READINGS];
} SensorData;

// Make an array of sensors
static SensorData sensors[NUM_SENSORS] = {
  {SCORE_SENSOR0, false, DEFAULT_THRESHOLD, {0, 0, 0}},
  {SCORE_SENSOR1, false, DEFAULT_THRESHOLD, {0, 0, 0}},
  {SCORE_SENSOR2, false, DEFAULT_THRESHOLD, {0, 0, 0}},
  {SCORE_SENSOR3, false, DEFAULT_THRESHOLD, {0, 0, 0}}			
};

/**
 * @inherit-doc
 */
void init_scoring() {
  for (int i = 0; i < NUM_SENSORS; i += 1) {
    pinMode(sensors[i].pin, INPUT);
  }
}

/**
 * @inherit-doc
 */
void update_threshold(uint8_t goal_num) {
  sensors[goal_num].threshold = THRESHOLD_MULTIPLIER * analogRead(sensors[goal_num].pin);
}

/**
 * @inherit-doc
 */
void process_scoring(void (*swrite)(char *)) {
  // Setup a buffer
  char buffer[SCORING_BUFFER_LENGTH];
  memset(buffer, 0, SCORING_BUFFER_LENGTH); // clear the buffer

  // Update all the readings and process goals that are now marked as scored
  for (int i = 0; i < NUM_SENSORS; i += 1) {
    // Shift data down in the array and the new data in
    sensors[i].readings[0] = sensors[i].readings[1];
    sensors[i].readings[1] = sensors[i].readings[2];
    sensors[i].readings[2] = analogRead(sensors[i].pin);

    // Check the readings to see if this is scored
    boolean s = true;
    for (int j = 0; j < NUM_READINGS; j += 1) {
      s = s && (sensors[i].readings[j] >= sensors[i].threshold);
    }

    // See if there's a state change
    if (sensors[i].scored && !s) {
      sensors[i].scored = false;
    } else if (!sensors[i].scored && s) {
      sensors[i].scored = true;
      sprintf(buffer, SCORE_DATA, i);
      (*swrite)(buffer);
    }
  }
}
