/**
 * src/serial/usbserial.hpp
 * 
 * Code for Serial (serial over USB) interactions with the FMS
 * 
 * @author Connor Henley
 */
#ifndef _usbserial_asc
#define _usbserial_asc

#define SERIAL_BUFFER_SIZE 128

/**
 * Intialize Serial over USB
 */
void init_usbser();

/**
 * Check Serial over USB for incoming data
 */
void check_usbser();

/**
 * Transmit data over Serial over USB
 * 
 * @note polls until data is sent
 */
void send_usbser(char * data);

#endif
