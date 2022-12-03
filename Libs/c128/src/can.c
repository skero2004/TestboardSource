#include <avr/io.h>
#include "can.h"

//MOB = message object (the number of mailboxes to transmit and receive messages)
#define MOB_COUNT 17	//ADDED 2 TO THIS NUMBER BC WE ADDED 2 NEW MESSAGES (USED TO BE 15)
//Each mob is defined as a struct

#if F_CPU != 16000000UL
	#error only clock rate of 16000000 is supported
#endif

typedef struct {
	uint8_t mode[MOB_COUNT];
	uint16_t id[MOB_COUNT];		//NOTE THIS IS A MAJOR CHANGE; USED TO BE uint16_t, BUT WE CHANGED TO 64 SO THAT IT CAN WORK WITH THE 32-BIT ADDRESS OF THE CAN CHARGER
	uint8_t len[MOB_COUNT];
} Config;

static Config config;

void canSetConfig(uint8_t index, uint8_t mode, uint16_t id, uint8_t len) {
	config.mode[index] = mode;
	config.id[index] = id;
	config.len[index] = len;
}

void canInit(void) {
	DDRD |= 0x80;	//INITIALIZES SOME PORT (port D7?) TO OUTPUT       //data direction register, group d

	// bit timing; CANBTx are specific to baud rate 125kbps
	CANBT1 = 0x0E;
	CANBT2 = 0x0C;
	CANBT3 = 0x37;
	
	//configure MObi     (i is the for loop counter)
	for (uint8_t i = 0; i < MOB_COUNT; i++) {
		CANPAGE = (i<<4);

		CANIDT1 = (config.id[i]>>3);
		CANIDT2 = (config.id[i]<<5);
		CANIDT3 = 0;
		CANIDT4 = 0;

		CANIDM1 = 0xFF;
		CANIDM2 = 0xE0;
		CANIDM3 = 0x00;
		CANIDM4 = 0x05;

		CANSTMOB = 0;
		CANCDMOB = (config.len[i]);

		if (config.mode[i] == CAN_RX) {
			CANCDMOB |= (1<<CONMOB1);
		}
	}

	CANGCON = (1<<ENASTB);
	while (!(CANGSTA & (1<<ENFG)));
}

bool canAvailable(uint8_t index) {
	if (index < 8) return !(CANEN2 & (1<<index));
	else return !(CANEN1 & (1<<(index-8)));
}

void canRead(uint8_t index, uint8_t *data) {
	CANPAGE = (index<<4);
	for (uint8_t i = 0; i < config.len[index]; i++) {
		*(data++) = CANMSG;
	}
	CANCDMOB |= (1<<CONMOB1);
}

void canWrite(uint8_t index, uint8_t *data) {
	if (canAvailable(index)) {
		CANPAGE = (index<<4);
		for (uint8_t i = 0; i < config.len[index]; i++) {
			CANMSG = *(data++);
		}
		CANCDMOB |= (1<<CONMOB0);
	}
}
