#ifndef POLL_H
#define POLL_H

#include "state.h"
#include "msg.h"

void pollInit(void);
void pollCharger(MsgChargeStat *msgChargeStat);

#endif
