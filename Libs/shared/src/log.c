#include "log.h"

#define LOG_ENCODING_START 12
#define LOG_ENCODING_LEN 80

#define BATTLOG_ENCODING_LEN 202

static const char LOG_TEMPLATE[] PROGMEM = "  :  :  .                                                                                   \n";
static const char BATTLOG_TEMPLATE[] PROGMEM = "                                                                                                                                                                                                          \n";

static void dataEncode(char *data, uint8_t start, uint8_t len);
static bool dataDecode(char *data, uint8_t start, uint8_t len);

static void logWrite(Log *log, char *data);
static void logRead(Log *log, char *data);
static void battLogWrite(BattLog *log, char *data);
static void battLogRead(BattLog *log, char *data);

static void writeUnsigned(char *data, uint32_t x, uint16_t pos, uint8_t n);
static void writeSigned(char *data, int32_t x, uint16_t pos, uint8_t n);
static void writeBool(char *data, bool x, uint16_t pos);

static uint32_t readUnsigned(char *data, uint16_t pos, uint8_t n);
static int32_t readSigned(char *data, uint16_t pos, uint8_t n);
static bool readBool(char *data, uint16_t pos);

static uint8_t getIndex(uint16_t pos);
static uint8_t getCount(uint16_t pos);

void logEncode(Log *log, char *data) {
	sutilWritePgm(data, 0, LOG_TEMPLATE);

	sutilWriteUInt(data, 0, log->elapsed / 360000, 2);
	sutilWriteUInt(data, 3, (log->elapsed % 360000) / 6000, 2);
	sutilWriteUInt(data, 6, (log->elapsed % 6000) / 100, 2);
	sutilWriteUInt(data, 9, log->elapsed % 100, 2);

	for (uint8_t i = 0; i < LOG_ENCODING_LEN; i++) {
		data[LOG_ENCODING_START + i] = 0;
	}

	logWrite(log, data+LOG_ENCODING_START);

    dataEncode(data, LOG_ENCODING_START, LOG_ENCODING_LEN);
}

bool logDecode (Log *log, char *data) {
	if (data[2] != ':' || data[5] != ':' || data[8] != '.'
		|| data[11] != ' ' || data[LOG_LEN-1] != '\n') {
		return false;
	}
    if (!dataDecode(data, LOG_ENCODING_START, LOG_ENCODING_LEN)) {
        return false;
    } 

	log->elapsed = 0;
	log->elapsed += sutilReadUInt(data, 9, 2);
	log->elapsed += sutilReadUInt(data, 6, 2) * 100;
	log->elapsed += sutilReadUInt(data, 3, 2) * 6000;
	log->elapsed += sutilReadUInt(data, 0, 2) * 360000;

	logRead(log, data+LOG_ENCODING_START);
	return true;
}

void battLogEncode(BattLog *log, char *data) {
	sutilWritePgm(data, 0, BATTLOG_TEMPLATE);

	for (uint8_t i = 0; i < BATTLOG_ENCODING_LEN; i++) {
		data[i] = 0;
	}

	battLogWrite(log, data);

    dataEncode(data, 0, BATTLOG_ENCODING_LEN);
}

bool battLogDecode (BattLog *log, char *data) {
	if (data[BATTLOG_LEN-1] != '\n') {
		return false;
	}
    if (!dataDecode(data, 0, BATTLOG_ENCODING_LEN)) {
        return false;
    } 

	battLogRead(log, data);
	return true;
}

static void logWrite (Log *log, char *data) {
	writeUnsigned(data, log->date, 0, 20);
	writeUnsigned(data, log->time, 20, 20);
	writeSigned(data, log->latitude, 40, 28);
	writeSigned(data, log->longitude, 68, 29);
	writeSigned(data, log->altitude, 97, 16);

	writeSigned(data, log->velocity, 113, 11);
	writeUnsigned(data, log->tempMotor, 124, 8);
	writeUnsigned(data, log->limitFlags, 132, 7);
	writeUnsigned(data, log->errorFlags, 139, 9);

	writeUnsigned(data, log->curDrive, 148, 10);
	writeSigned(data, log->velDrive, 158, 11);
	writeUnsigned(data, log->velCruise, 169, 10);
	writeUnsigned(data, log->pedal, 179, 10);
	writeUnsigned(data, log->regen, 189, 10);
	writeUnsigned(data, log->pressureA, 199, 11);
	writeUnsigned(data, log->pressureB, 210, 11);
	writeUnsigned(data, log->pressureC, 221, 11);
	writeUnsigned(data, log->pressureD, 232, 11);
	writeBool(data, log->wheelFlag, 243);
	writeBool(data, log->dirFlag, 244);
	writeBool(data, log->cruiseFlag, 245);

	writeUnsigned(data, log->voltSuppExt, 246, 11);
	writeBool(data, log->brakeFlag, 257);
	writeBool(data, log->leftFlag, 258);
	writeBool(data, log->rightFlag, 259);
	writeBool(data, log->hazardFlag, 260);
	writeBool(data, log->hlightFlag, 261);
	writeBool(data, log->mfanFlag, 262);
	writeBool(data, log->dfanFlag, 263);

	writeUnsigned(data, log->mode, 264, 4);
	writeBool(data, log->balState, 268);
	writeUnsigned(data, log->bfanState, 269, 8);
	writeUnsigned(data, log->voltSuppInt, 277, 11);
	writeUnsigned(data, log->voltMin, 288, 16);
	writeUnsigned(data, log->voltAv, 304, 16);
	writeUnsigned(data, log->voltMax, 320, 16);
	writeUnsigned(data, log->tempMin, 336, 8);
	writeUnsigned(data, log->tempAv, 344, 8);
	writeUnsigned(data, log->tempMax, 352, 8);
	writeUnsigned(data, log->airTemp, 360, 8);
    writeSigned(data, log->soc, 368, 32);

	writeSigned(data, log->curSystems, 400, 11);
	writeSigned(data, log->curBattery, 411, 11);
	writeSigned(data, log->curMotor, 422, 11);
	writeSigned(data, log->curArray, 433, 11);
	writeBool(data, log->systemsState, 444);
	writeBool(data, log->motorState, 445);
	writeBool(data, log->arrayState, 446);

    writeUnsigned(data, log->spareA, 447, 8);
    writeUnsigned(data, log->spareB, 455, 8);
    writeUnsigned(data, log->spareC, 463, 8);
    writeUnsigned(data, log->spareD, 471, 8);
}

static void logRead(Log *log, char *data) {
	log->date = readUnsigned(data, 0, 20);
	log->time = readUnsigned(data, 20, 20);
	log->latitude = readSigned(data, 40, 28);
	log->longitude = readSigned(data, 68, 29);
	log->altitude = readSigned(data, 97, 16);

	log->velocity = readSigned(data, 113, 11);
	log->tempMotor = readUnsigned(data, 124, 8);
	log->limitFlags = readUnsigned(data, 132, 7);
	log->errorFlags = readUnsigned(data, 139, 9);

	log->curDrive = readUnsigned(data, 148, 10);
	log->velDrive = readSigned(data, 158, 11);
	log->velCruise = readUnsigned(data, 169, 10);
	log->pedal = readUnsigned(data, 179, 10);
	log->regen = readUnsigned(data, 189, 10);
	log->pressureA = readUnsigned(data, 199, 11);
	log->pressureB = readUnsigned(data, 210, 11);
	log->pressureC = readUnsigned(data, 221, 11);
	log->pressureD = readUnsigned(data, 232, 11);
	log->wheelFlag = readBool(data, 243);
	log->dirFlag = readBool(data, 244);
	log->cruiseFlag = readBool(data, 245);

	log->voltSuppExt = readUnsigned(data, 246, 11);
	log->brakeFlag = readBool(data, 257);
	log->leftFlag = readBool(data, 258);
	log->rightFlag = readBool(data, 259);
	log->hazardFlag = readBool(data, 260);
	log->hlightFlag = readBool(data, 261);
	log->mfanFlag = readBool(data, 262);
    log->dfanFlag = readBool(data, 263);

	log->mode = readUnsigned(data, 264, 4);
	log->balState = readBool(data, 268);
	log->bfanState = readUnsigned(data, 269, 8);
	log->voltSuppInt = readUnsigned(data, 277, 11);
	log->voltMin = readUnsigned(data, 288, 16);
	log->voltAv = readUnsigned(data, 304, 16);
	log->voltMax = readUnsigned(data, 320, 16);
	log->tempMin = readUnsigned(data, 336, 8);
	log->tempAv = readUnsigned(data, 344, 8);
	log->tempMax = readUnsigned(data, 352, 8);
	log->airTemp = readUnsigned(data, 360, 8);
	log->soc = readSigned(data, 368, 32);

	log->curSystems = readSigned(data, 400, 11);
	log->curBattery = readSigned(data, 411, 11);
	log->curMotor = readSigned(data, 422, 11);
	log->curArray = readSigned(data, 433, 11);
	log->systemsState = readBool(data, 444);
	log->motorState = readBool(data, 445);
	log->arrayState = readBool(data, 446);

    log->spareA = readUnsigned(data, 447, 8);
    log->spareB = readUnsigned(data, 455, 8);
    log->spareC = readUnsigned(data, 463, 8);
    log->spareD = readUnsigned(data, 471, 8);
}

static void battLogWrite (BattLog *log, char *data) {
	writeUnsigned(data, log->mode, 0, 4);
	writeBool(data, log->balState, 4);
	writeUnsigned(data, log->bfanState, 5, 8);
	writeUnsigned(data, log->voltSuppInt, 13, 11);
	writeUnsigned(data, log->voltMin, 24, 16);
	writeUnsigned(data, log->voltAv, 40, 16);
	writeUnsigned(data, log->voltMax, 56, 16);
	writeUnsigned(data, log->tempMin, 72, 8);
	writeUnsigned(data, log->tempAv, 80, 8);
	writeUnsigned(data, log->tempMax, 88, 8);
    writeUnsigned(data, log->airTemp, 96, 8);
    writeSigned(data, log->soc, 104, 32);

	writeSigned(data, log->curSystems, 136, 11);
	writeSigned(data, log->curBattery, 147, 11);
	writeSigned(data, log->curMotor, 158, 11);
	writeSigned(data, log->curArray, 169, 11);
	writeBool(data, log->motorState, 180);
	writeBool(data, log->arrayState, 181);
	writeBool(data, log->systemsState, 182);

    for (uint8_t i = 0; i < CELL_COUNT*MODULE_COUNT; i++) {
        writeUnsigned(data, log->voltArr[i], 183+(i<<4), 16);
    }
    for (uint8_t i = 0; i < CELL_COUNT*MODULE_COUNT; i++) {
        writeUnsigned(data, log->tempArr[i], 183+((CELL_COUNT*MODULE_COUNT)<<4)+(i<<3), 8);
    }
    for (uint8_t i = 0; i < CELL_COUNT*MODULE_COUNT; i++) {
        writeUnsigned(data, log->balArr[i], 183+(CELL_COUNT*MODULE_COUNT*24)+(i<<3), 8);
    }
    writeUnsigned(data, log->page, 1207, 3);
}

static void battLogRead(BattLog *log, char *data) {
	log->mode = readUnsigned(data, 0, 4);
	log->balState = readBool(data, 4);
	log->bfanState = readUnsigned(data, 5, 8);
	log->voltSuppInt = readUnsigned(data, 13, 11);
	log->voltMin = readUnsigned(data, 24, 16);
	log->voltAv = readUnsigned(data, 40, 16);
	log->voltMax = readUnsigned(data, 56, 16);
	log->tempMin = readUnsigned(data, 72, 8);
	log->tempAv = readUnsigned(data, 80, 8);
	log->tempMax = readUnsigned(data, 88, 8);
    log->airTemp = readUnsigned(data, 96, 8);
	log->soc = readSigned(data, 104, 32);

	log->curSystems = readSigned(data, 136, 11);
	log->curBattery = readSigned(data, 147, 11);
	log->curMotor = readSigned(data, 158, 11);
	log->curArray = readSigned(data, 169, 11);
	log->motorState = readBool(data, 180);
	log->arrayState = readBool(data, 181);
	log->systemsState = readBool(data, 182);

    for (uint8_t i = 0; i < CELL_COUNT*MODULE_COUNT; i++) {
        log->voltArr[i] = readUnsigned(data, 183+(i<<4), 16);
    }
    for (uint8_t i = 0; i < CELL_COUNT*MODULE_COUNT; i++) {
        log->tempArr[i] = readUnsigned(data, 183+((CELL_COUNT*MODULE_COUNT)<<4)+(i<<3), 8);
    }
    for (uint8_t i = 0; i < CELL_COUNT*MODULE_COUNT; i++) {
        log->balArr[i] = readUnsigned(data, 183+(CELL_COUNT*MODULE_COUNT*24)+(i<<3), 8);
    }
    
    log->page = readUnsigned(data, 1207, 3);
}

static void dataEncode(char *data, uint8_t start, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		uint8_t x = data[start + i];
		uint8_t c;

		if (x < 26) c = 'A' + x;
		else if (x < 52) c = 'a' + x - 26;
		else if (x < 62) c = '0' + x - 52;
		else if (x == 62) c = '+';
		else c = '/';

		data[start + i] = c;
	}
}

static bool dataDecode(char *data, uint8_t start, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		uint8_t c = data[start + i];
		uint8_t x;

		if (c >= 'A' && c <= 'Z') x = c - 'A';
		else if (c >= 'a' && c <= 'z') x = c - 'a' + 26;
		else if (c >= '0' && c <= '9') x = c - '0' + 52;
		else if (c == '+') x = 62;
		else if (c == '/') x = 63;
		else return false;

		data[start + i] = x;
	}
    return true;
}

static void writeUnsigned(char *data, uint32_t x, uint16_t pos, uint8_t n) {
	for (uint8_t i = 0; i < n; i++) {
		writeBool(data, x & 1, pos+i);
		x >>= 1;
	}
}

static void writeSigned(char *data, int32_t x, uint16_t pos, uint8_t n) {
	writeUnsigned(data, x, pos, n);
}

static void writeBool(char *data, bool x, uint16_t pos) {
	if (x) {
		data[getIndex(pos)] |= (1 << getCount(pos));
	}
}

static uint32_t readUnsigned(char *data, uint16_t pos, uint8_t n) {
	uint32_t x = 0;
	for (uint8_t i = 0; i < n; i++) {
		x <<= 1;
		if (readBool(data, pos+n-1-i)) {
			x |= 1;
		}
	}
	return x;
}

static int32_t readSigned(char *data, uint16_t pos, uint8_t n) {
	int32_t x = 0;
	if (readBool(data, pos+n-1)) {
		x = -1;
	}
	for (uint8_t i = 1; i < n; i++) {
		x <<= 1;
		if (readBool(data, pos+n-1-i)) {
			x |= 1;
		}
	}
	return x;
}

static bool readBool(char *data, uint16_t pos) {
	if (data[getIndex(pos)] & (1 << getCount(pos))) {
		return true;
	} else {
		return false;
	}
}

static uint8_t getIndex(uint16_t pos) {
	return (pos / 6);
}

static uint8_t getCount(uint16_t pos) {
	return (pos % 6);
}
