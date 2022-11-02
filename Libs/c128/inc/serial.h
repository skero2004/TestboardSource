#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdbool.h>

void serialInit(void);
bool serialAvailable(uint8_t index);
uint8_t serialRead(uint8_t index);
void serialWrite(uint8_t index, uint8_t data);

#endif
