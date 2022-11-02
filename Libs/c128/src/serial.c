#include <avr/interrupt.h>
#include "serial.h"

#ifndef SERIAL0_BAUD
	#define SERIAL0_BAUD 9600
	#warning SERIAL0_BAUD set to 9600
#endif
#ifndef SERIAL1_BAUD
	#define SERIAL1_BAUD 9600
	#warning SERIAL1_BAUD set to 9600
#endif

#if F_CPU == 16000000UL
	#if SERIAL0_BAUD == 9600
		#define SERIAL0_BRR 207
	#elif SERIAL0_BAUD == 19200
		#define SERIAL0_BRR 103
	#elif SERIAL0_BAUD == 38400
		#define SERIAL0_BRR 51
	#else
		#error only baud rates of 9600, 19200, and 38400 are supported
	#endif

	#if SERIAL1_BAUD == 9600
		#define SERIAL1_BRR 207
	#elif SERIAL1_BAUD == 19200
		#define SERIAL1_BRR 103
	#elif SERIAL1_BAUD == 38400
		#define SERIAL1_BRR 51
	#else
		#error only baud rates of 9600, 19200, and 38400 are supported
	#endif
#else
	#error only clock rate of 16000000 is supported
#endif

#ifndef SERIAL0_RX_SIZE
	#define SERIAL0_RX_SIZE 32
	#warning SERIAL0_RX_SIZE set to 32
#endif
#ifndef SERIAL0_TX_SIZE
	#define SERIAL0_TX_SIZE 32
	#warning SERIAL0_TX_SIZE set to 32
#endif
#ifndef SERIAL1_RX_SIZE
	#define SERIAL1_RX_SIZE 32
	#warning SERIAL1_RX_SIZE set to 32
#endif
#ifndef SERIAL1_TX_SIZE
	#define SERIAL1_TX_SIZE 32
	#warning SERIAL1_TX_SIZE set to 32
#endif

typedef struct {
	volatile uint8_t *arr;
	volatile uint16_t head;
	volatile uint16_t tail;
	const uint16_t size;
} Buffer;

static volatile uint8_t arrRx0[SERIAL0_RX_SIZE];
static volatile uint8_t arrTx0[SERIAL0_TX_SIZE];
static volatile uint8_t arrRx1[SERIAL1_RX_SIZE];
static volatile uint8_t arrTx1[SERIAL1_TX_SIZE];

static Buffer rx0 = {arrRx0, 0, 0, SERIAL0_RX_SIZE};
static Buffer tx0 = {arrTx0, 0, 0, SERIAL0_TX_SIZE};
static Buffer rx1 = {arrRx1, 0, 0, SERIAL1_RX_SIZE};
static Buffer tx1 = {arrTx1, 0, 0, SERIAL1_TX_SIZE};

static void push(Buffer *buffer, uint8_t data);
static uint8_t pop(Buffer *buffer);
static bool isEmpty(Buffer *buffer);

void serialInit(void) {
	cli();

	UBRR0 = SERIAL0_BRR;
	UCSR0A = (1<<U2X);
	UCSR0B = (1<<RXEN)|(1<<RXCIE)|(1<<TXEN);
	UCSR0C = (1<<UCSZ0)|(1<<UCSZ1);

	UBRR1 = SERIAL1_BRR;
	UCSR1A = (1<<U2X);
	UCSR1B = (1<<RXEN)|(1<<RXCIE)|(1<<TXEN);
	UCSR1C = (1<<UCSZ0)|(1<<UCSZ1);

	sei();
}

bool serialAvailable(uint8_t index) {
	if (!index) return !isEmpty(&rx0);
	else return !isEmpty(&rx1);
}

uint8_t serialRead(uint8_t index) {
	uint8_t data;
	cli();
	if (!index) data = pop(&rx0);
	else data = pop(&rx1);
	sei();
	return data;
}

void serialWrite(uint8_t index, uint8_t data) {
	cli();
	if (!index) {
		if (UCSR0B & (1<<UDRIE)) {
			push(&tx0, data);
		} else {
			UDR0 = data;
			UCSR0B |= (1<<UDRIE);
		}
	} else {
		if (UCSR1B & (1<<UDRIE)) {
			push(&tx1, data);
		} else {
			UDR1 = data;
			UCSR1B |= (1<<UDRIE);
		}
	}
	sei();
}

ISR(USART0_RX_vect) {
	push(&rx0, UDR0);
}

ISR(USART0_UDRE_vect) {
	if (!isEmpty(&tx0)) {
		UDR0 = pop(&tx0);
	} else {
		UCSR0B &= ~(1<<UDRIE);
	}
}

ISR(USART1_RX_vect) {
	push(&rx1, UDR1);
}

ISR(USART1_UDRE_vect) {
	if (!isEmpty(&tx1)) {
		UDR1 = pop(&tx1);
	} else {
		UCSR1B &= ~(1<<UDRIE);
	}
}

static void push(Buffer *buffer, uint8_t data) {
	buffer->arr[buffer->head++] = data;
	if (buffer->head >= buffer->size) buffer->head = 0;
}

static uint8_t pop(Buffer *buffer) {
	uint8_t data = buffer->arr[buffer->tail++];
	if (buffer->tail >= buffer->size) buffer->tail = 0;
	return data;
}

static bool isEmpty(Buffer *buffer) {
	return (buffer->head == buffer->tail);
}
