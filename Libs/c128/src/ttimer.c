#include <avr/io.h>
#include <avr/interrupt.h>
#include "ttimer.h"

#if F_CPU != 16000000UL
	#error only clock rate of 16000000 is supported
#endif

#ifndef TTIMER_FREQUENCY
	#define TTIMER_FREQUENCY 4
	#warning TTIMER_FREQUENCY set to 4
#endif

#if TTIMER_FREQUENCY == 4
	#define TOP 0xF423
#elif TTIMER_FREQUENCY == 8
	#define TOP 0x7A11
#elif TTIMER_FREQUENCY == 16
	#define TOP 0x3D08
#elif TTIMER_FREQUENCY == 32
	#define TOP 0x1E83
#elif TTIMER_FREQUENCY == 64
	#define TOP 0x0F41
#elif TTIMER_FREQUENCY == 128
	#define TOP 0x07A0
#else
	#error only frequencies of 4, 8, 16, 32, 64, and 128 are supported
#endif

static volatile bool ready;

void ttimerInit(void) {
	cli();
	TCCR1A = 0;
	TCCR1B = (1<<WGM12)|(1<<WGM13);
	TCCR1C = 0;
	ICR1 = TOP;
	TIMSK1 = (1<<ICIE1);
	TCCR1B |= (1<<CS11)|(1<<CS10);
	sei();
}

bool ttimerPoll(void) {
	cli();
	if (ready) {
		ready = false;
		sei();
		return true;
	} else {
		sei();
		return false;
	}
}

ISR(TIMER1_CAPT_vect) {
	ready = true;
}
