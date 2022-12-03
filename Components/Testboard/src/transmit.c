#include "msg.h"
#include "transmit.h"



void transmitInit(void) {
	msgEnable(MSG_BATT_CHARGE, MSG_TX);
}

void transmit(MsgBattCharge *msgBattCharge) {
	msgWrite(MSG_BATT_CHARGE, &msgBattCharge);
}