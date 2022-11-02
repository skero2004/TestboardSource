#ifndef CTIMER_H
#define CTIMER_H

#include <stdint.h>
#include <stdbool.h>

void ctimerInit(uint16_t delay);
void ctimerReset(uint8_t index);
bool ctimerPoll(uint8_t index);

#endif
