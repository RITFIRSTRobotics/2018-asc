/**
 * src/usbserial.hpp
 * 
 * Code for Serial (serial over USB) interactions with the FMS
 * 
 * @author Connor Henley
 */
#ifndef _usbserial_asc
#define _usbserial_asc

/**
 * Intialize Serial over USB
 */
void init_usbser();

/**
 * Transmit data
 * 
 * @note polls until data is sent
 */
void send_usbser(char * data);

#endif
