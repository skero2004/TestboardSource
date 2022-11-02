#include "mode.h"

static char * MODE_LABELS[] = {
	"NL", "ST", "RN", "E1", "E2", "E3", "E4", "E5", "E6",
};

bool modeIsError(Mode mode) {
	return (mode >= MODE_ERROR_MANUAL_FAULT);
}

char * modeGetLabel(Mode mode) {
	return MODE_LABELS[mode];
}
