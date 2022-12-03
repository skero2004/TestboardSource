#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "serial.h"
#include "adc.h"
#include "log.h"
#include "msg.h"
#include "sutil.h"
#include "ttimer.h"
#include "ctimer.h"
// #include "state.h"

#define DELAY 250
#define STARTUP_DELAY 250

// StateCharge stateCharge;		// struct that is read into, and after reading is done, its values are edited and transmitted
MsgChargeStat msgChargeStat;
MsgBattCharge msgBattCharge; 	

int main(void) {
	ttimerInit();
	transmitInit();
	msgInit();
	_delay_ms(STARTUP_DELAY);

	while(1) {
		if (ttimerPoll()) {
			// pollCharge(&stateCharge);	//pass in memory address to stateCharge struct; pollCharge will write new values into the memory location of strateCharge
			pollCharge(&msgChargeStat);

			//NOW THE VALUES IN *stateCharge struct have been updated; read these values and perform actions as needed
			//i.e. if (stateCharge.current > 1) stopChargeOrSomethingLikeThat();


			

			//now that the read values in stateCharge have been used and processed, edit values of msgBattCharge to become the message we want to transmit
			msgBattCharge.charge_cur = msgChargeStat.charge_cur;
			msgBattCharge.charge_voltage = msgChargeStat.charge_voltage;	//passing back in the same values we read
			msgBattCharge.control = 1;
			transmit(&msgBattCharge);
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
