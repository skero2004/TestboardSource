#include <avr/io.h>
#include <string.h>
#include "msg.h"
// #include "serial.h"
// #include "adc.h"
#include "ctimer.h"
#include "poll.h"

#define CLEAR_DELAY 500

void pollInit(void) {
	msgEnable(MSG_CHARGE_STAT, MSG_RX);
	// Serial and ADC aren't being used by CAN Charger
	// serialInit();
	// adcInit();
	ctimerInit(CLEAR_DELAY);

	DDRA = 0x00;
	DDRD = 0x02;
}

void pollCharge(MsgChargeStat *msgChargeStat) {
	if (msgAvailable(MSG_CHARGE_STAT)) {	//runs canAvailable() on the message type MSG_CHARGE_STAT
		msgRead(MSG_CHARGE_STAT, msgChargeStat);
		ctimerReset(0);
	}

	if (ctimerPoll(0)) {
		//Setting every bit in memory taken up by msgChargeStat to 0 (effectively erasing the data that was there before)
		memset(msgChargeStat, 0, sizeof(MsgChargeStat));
	}
}