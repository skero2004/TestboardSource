#ifndef MODE_H
#define MODE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	MODE_NULL,
	MODE_STARTUP,
	MODE_RUN,
	MODE_ERROR_MANUAL_FAULT,
	MODE_ERROR_UNDER_VOLTAGE,
	MODE_ERROR_OVER_VOLTAGE,
	MODE_ERROR_OVER_CURRENT,
	MODE_ERROR_OVER_TEMPERATURE,
	MODE_ERROR_SIGNAL_FAULT,
} Mode;

bool modeIsError(Mode mode);
char * modeGetLabel(Mode mode);

#endif
