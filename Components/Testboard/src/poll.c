#include <avr/io.h>
#include <string.h>
#include "msg.h"
#include "serial.h"
#include "adc.h"
#include "ctimer.h"
#include "poll.h"

void pollInit(void) {
	msgEnable(MSG_BATT_STAT, MSG_RX);
	msgEnable(MSG_MOTOR_VEL, MSG_RX);
	serialInit();
	adcInit();
	ctimerInit(CLEAR_DELAY);

	DDRA = 0x00;
	DDRD = 0x02;
}

void pollCharge(MsgChargeStat *msgChargeStat) {
	if (msgAvailable(MSG_CHARGE_STAT)) {
		msgRead(MSG_CHARGE_STAT, msgChargeStat);
		ctimerReset(0); //IDK WHAT THIS IS FOR
	}

    //NO IDEA WHAT THIS IF STATEMENT DOES
	if (ctimerPoll(0)) {
		memset(msgChargeStat, 0, sizeof(MsgChargeStat));
	}
}