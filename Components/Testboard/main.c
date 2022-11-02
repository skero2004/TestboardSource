#include <util/delay.h>
#include "msg.h"

#define DELAY 250

MsgBattSOC msgBattSoc;

int main(void) {
	msgEnable(MSG_BATT_SOC, MSG_TX);
	msgInit();

	msgBattSoc.soc = 88;

	while(1) {
		msgWrite(MSG_BATT_SOC, &msgBattSoc);
		_delay_ms(DELAY);
	}

	return 0;
}
