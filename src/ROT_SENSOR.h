/*
    Module dedicated for control over AS5600 12-Bit Programmable Contactless Potentiometer
*/

#include <avr/io.h>
#include <TWI.h>

#define AS5600_ADDRESS 0x36
#define AS5600_STATUS 0x0B   // 5 - Magnet was detected 4 - Magnet too weak 3 - Magnet too strong
#define AS5600_ANGLE_H 0x0C  // Angle most significant bits
#define AS5600_ANGLE_L 0x0D  // Angle least significant bits
#define AS5600_AGC 0x1A      // AGC, gain

/*
    Single byte write sequence
*/
void writeToRegister(uint8_t registerAddres, uint8_t value);
uint8_t readRegister(uint8_t registerAddress, bool ACK);
uint16_t getAngle();