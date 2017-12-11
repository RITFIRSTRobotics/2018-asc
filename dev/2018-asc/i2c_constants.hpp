/**
 * i2c/i2c_constants.hpp
 *
 * Constants for i2c (I squared C) transmission between the controller and the Arduino Scoring Controller (ASC)
 *
 * @author Connor Henley, @thatging3rkid
 */
#ifndef _i2c_constants_hpp
#define _i2c_constants_hpp

#define BASE_ADDR 0x10 // Base i2c address (addresses should add to this)

// #defines for the the addresses
#define ADDR0 BASE_ADDR + 1
#define ADDR1 BASE_ADDR + 2
#define ADDR2 BASE_ADDR + 3

// #defines for transmission data
#define I2CDATA_DELIMITER ":"
#define I2CDATA_BUFFER_LEN 60 // len of format string should be ~26
// formatting string, should evaluate to "d:%03d:...:%01d:..."
#define I2CDATA_FORMAT_STRING "d" I2CDATA_DELIMITER "%03d" I2CDATA_DELIMITER "%03d" \
    I2CDATA_DELIMITER "%03d" I2CDATA_DELIMITER "%03d" I2CDATA_DELIMITER "%01d" \
    I2CDATA_DELIMITER "%01d" I2CDATA_DELIMITER "%01d" I2CDATA_DELIMITER "%01d" \
    I2CDATA_DELIMITER

#endif
