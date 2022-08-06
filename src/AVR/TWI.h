#ifndef _TWI_H_
#define _TWI_H_
#include <stdbool.h>

#define WRITE_BYTE 0
#define READ_BYTE 1

#define SEND_ACK 1
#define SEND_NACK 0

#define LOG_ENABLED 1
#define LOG_DISABLED 0

void TWI_init(bool log);
void TWI_start(void);
void TWI_sendAddress(unsigned char address, unsigned char direction);
void TWI_sendData(unsigned char data);
unsigned char TWI_getData(bool ack);
void TWI_stop(void);

#endif /* _TWI_H_ */