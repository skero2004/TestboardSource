#include "msg.h"
#include "transmit.h"

static MsgBattCharge msgBattCharge; 

void transmitInit(void) {
	msgEnable(MSG_BATT_CHARGE, MSG_TX);
}

void transmit() {
	//Define all the characteristics of msgBattCharge here
	//msgBattCharge.charge_cur = 5;
	msgWrite(MSG_BATT_CHARGE, &msgBattCharge);
}