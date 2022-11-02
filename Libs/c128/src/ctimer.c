#include <avr/io.h> 
#include <avr/interrupt.h>
#include "ctimer.h"

#if F_CPU == 16000000UL
	#define TOP 0xF9
#else
	#error only clock rate of 16000000 is supported
#endif

#ifndef CTIMER_CHANNELS
	#define CTIMER_CHANNELS 1
	#warning CTIMER_CHANNELS set to 1
#endif

static uint16_t top;
static volatile uint16_t ticks[CTIMER_CHANNELS];
static bool cleared[CTIMER_CHANNELS];

void ctimerInit(uint16_t delay) {
	top = delay;
	for (uint8_t i = 0; i < CTIMER_CHANNELS; i++) {
		cleared[i] = true;
	}

	cli();
	TCCR0A = (1<<WGM01)|(1<<CS01)|(1<<CS00);
	TIMSK0 = (1<<OCIE0A);
	OCR0A = TOP; // 1ms
	sei();
}

void ctimerReset(uint8_t index) {
	cli();
	ticks[index] = 0;
	sei();

	cleared[index] = false;
}

bool ctimerPoll(uint8_t index) {
	cli();
	if (ticks[index] >= top && !cleared[index]) {
		sei();
		cleared[index] = true;
		return true;
	} else {
		sei();
		return false;
	}
}

ISR(TIMER0_COMP_vect) {
	for (uint8_t i = 0; i < CTIMER_CHANNELS; i++) {
		if (ticks[i] < top) ticks[i]++;
	}
}
