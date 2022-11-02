#include <avr/io.h>
#include "adc.h"

#if F_CPU != 16000000UL
	#error only clock rate of 16000000 is supported
#endif

void adcInit(void) {
	ADCSRA = (1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
}

uint16_t adcRead(uint8_t index) {
	ADMUX = index & 0x07;
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	return ADC;
}
