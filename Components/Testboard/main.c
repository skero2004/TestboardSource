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

#define DELAY 250
#define STARTUP_DELAY 250

MsgBattCharge msgBattCharge;

int main(void) {
	_delay_ms(STARTUP_DELAY);
	// timerInit();
	transmitInit();
	msgInit();

	while(1) {
		if (ttimerPoll()) {
			transmit();
		}
	}
}
