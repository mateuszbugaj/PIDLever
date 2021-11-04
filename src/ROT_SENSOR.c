#include <ROT_SENSOR.h>

void writeToRegister(uint8_t registerAddres, uint8_t value) {
    TWI_start();
    TWI_sendAddress(AS5600_ADDRESS, WRITE_BYTE);
    TWI_sendData(registerAddres);
    TWI_sendData(value);
    TWI_stop();
}

uint8_t readRegister(uint8_t registerAddress, bool ACK) {
    uint8_t value;

    TWI_start();
    TWI_sendAddress(AS5600_ADDRESS, WRITE_BYTE);
    TWI_sendData(registerAddress);
    TWI_start();
    TWI_sendAddress(AS5600_ADDRESS, READ_BYTE);
    value = TWI_getData(ACK);
    TWI_stop();

    return value;
}

uint16_t getAngle() {
    uint8_t angle_H, angle_L;

    angle_H = readRegister(AS5600_ANGLE_H, SEND_NACK);
    angle_L = readRegister(AS5600_ANGLE_L, SEND_NACK);

    return (angle_H << 8) | angle_L;
}

uint8_t getAS5600Status(){
    return readRegister(AS5600_STATUS, SEND_NACK);
}
