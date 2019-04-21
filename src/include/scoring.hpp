/**
 * src/include/scoring.hpp
 * 
 * Code for the scoring subsystem of the ASC
 * 
 * @author Connor Henley, @thatging3rkid
 */
#ifndef _scoring_asc
#define _scoring_asc

#include <stdint.h>

#define DEFAULT_THRESHOLD 1000
#define THRESHOLD_MULTIPLIER 1.1

/**
 * Initialize the scoring sub-system
 */
void init_scoring();

/**
 * Update the threshold for a goal
 * 
 * @param goal_num the goal number to update
 */
void update_threshold(uint8_t goal_num);

/**
 * See if a goal has been scored, and if so, report it
 * 
 * @param swrite a function to write the data (without a newline) to
 */
void process_scoring(void (*swrite)(char *));

#endif

