#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "serial.h"
#include "adc.h"
#include "log.h"
#include "msg.h"
#include "sutil.h"
// #include "ttimer.h"
#include "ttimer.h"
#include "ctimer.h"

#define DELAY 250
#define STARTUP_DELAY 250

MsgBattCharge msgBattCharge;

StateCharge stateCharge;

int main(void) {
	ttimerInit();
	transmitInit();
	msgInit();
	_delay_ms(STARTUP_DELAY);

	while(1) {
		if (ttimerPoll()) {
			// if (msgAvailable(MSG_BATT_CHARGE)) {
			// 	msgRead(MSG_BATT_CHARGE, &msgBattCharge);
			// 	dashLog.mode = msgBattStat.mode;
			// 	dashLog.balState = msgBattStat.balState;
			// 	dashLog.bfanState = msgBattStat.bfanState;
			// 	dashLog.motorState = msgBattStat.motorState;
			// 	dashLog.arrayState = msgBattStat.arrayState;
			// 	dashLog.systemsState = msgBattStat.systemsState;
			// 	ctimerReset(0);
			// }
			stateCharge.current = 1;
			stateCharge.voltage = 12;	//random numbers for now
			transmit(&stateCharge);
		}
	}
}


/*
NOTES
- the same MSG can be set to TX and RX by different boards, depending on whether they write the message or need to read from it
	- example: MSG_BATT_STAT is written to by the battery control board (TX), but read by multiple other boards that want to know if there's a battery error
- create new MSG for receiving from the CAN charger (this will be in RX mode for all boards)
	- to enable RX functionality, create and implement poll.c and poll.h
- 
*/
