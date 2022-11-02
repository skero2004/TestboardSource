#ifndef CAN_H
#define CAN_H

#include <stdint.h>
#include <stdbool.h>

#define CAN_RX 1
#define CAN_TX 2

void canSetConfig(uint8_t index, uint8_t mode, uint16_t id, uint8_t len);
void canInit(void);
bool canAvailable(uint8_t index);
void canRead(uint8_t index, uint8_t *data);
void canWrite(uint8_t index, uint8_t *data);

#endif
