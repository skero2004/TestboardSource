#include "msg.h"
#include "transmit.h"

static MsgBattCharge msgBattCharge; 

void transmitInit(void) {
	msgEnable(MSG_BATT_CHARGE, MSG_TX);
}

void transmit(StateCharge *stateCharge) {
	//Define all the characteristics of msgBattCharge here
	msgBattCharge.charge_cur = stateCharge->current;
	msgBattCharge.charge_voltage = stateCharge->voltage;
	msgWrite(MSG_BATT_CHARGE, &msgBattCharge);
}