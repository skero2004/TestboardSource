#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	int16_t current;    //not sure if int16_t is the right type
    int16_t voltage;
} StateCharge;

#endif