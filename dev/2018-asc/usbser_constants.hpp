/**
 * serial/usbser_Constants.hpp
 *
 * A bunch of constants used for serial transmission between the FMS and the scoring Arduino
 */
#ifndef _serial_constants_hpp
#define _serial_constants_hpp

#define BAUD_RATE 115200 // Baud Rate of the connection

#define NEWLINE '\n' // newline character and end-of-line character

/**
 * Transmitted codes
 *
 * These codes are designed to be as short as possible in the name of speed
 */

#define INIT_STRING "i0:%c" // initialization string (replace %c with color (r or b))

#define TEST_MESSAGE "t0:t" // test if anyone is home
#define TEST_RESPONSE "t0:s" // test successful

#define MOVEMENT_DATA "md:%b:%b" // movement data of the two sticks (%b is an unsigned byte)

#endif
