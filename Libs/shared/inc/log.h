#ifndef LOG_H
#define LOG_H

#include <stdint.h>
#include <stdbool.h>
#include "mode.h"
#include "const.h"
#include "sutil.h"

#define LOG_LEN 93
#define BATTLOG_LEN 203

typedef struct {
	// Locale log
	uint32_t elapsed;
	uint32_t date;
	uint32_t time;
	int32_t latitude;
	int32_t longitude;
	int16_t altitude;

	// Motor log
	int16_t velocity;
	uint8_t tempMotor;
	uint8_t limitFlags;
	uint16_t errorFlags;

	// Drive log
	uint16_t curDrive;
	int16_t velDrive;
	uint16_t velCruise;
	uint16_t pedal;
	uint16_t regen;
	uint16_t pressureA;
	uint16_t pressureB;
	uint16_t pressureC;
	uint16_t pressureD;
	bool wheelFlag;
	bool dirFlag;
	bool cruiseFlag;

	// Peripherals log
	uint16_t voltSuppExt;
	bool brakeFlag;
	bool leftFlag;
	bool rightFlag;
	bool hazardFlag;
	bool hlightFlag;
	bool mfanFlag;
	bool dfanFlag;

	// Battery log
	Mode mode;
	bool balState;
	uint8_t bfanState;
	uint16_t voltSuppInt;
	uint16_t voltMin;
	uint16_t voltAv;
	uint16_t voltMax;
	uint8_t tempMin;
	uint8_t tempAv;
	uint8_t tempMax;
    uint8_t airTemp;
    int32_t soc;

	// Current log
	int16_t curSystems;
	int16_t curBattery;
	int16_t curMotor;
	int16_t curArray;
    bool systemsState;
	bool motorState;
	bool arrayState;

    // spare fields
    uint8_t spareA;
    uint8_t spareB;
    uint8_t spareC;
    uint8_t spareD;
} Log;

typedef struct {
    // status log
	Mode mode;
	bool balState;
	uint8_t bfanState;
	uint16_t voltSuppInt;
	uint16_t voltMin;
	uint16_t voltAv;
	uint16_t voltMax;
	uint8_t tempMin;
	uint8_t tempAv;
	uint8_t tempMax;
    uint8_t airTemp;
    int32_t soc;

    // current log
	int16_t curSystems;
	int16_t curBattery;
	int16_t curMotor;
	int16_t curArray;
	bool motorState;
	bool arrayState;
    bool systemsState;

    // cell log
    uint16_t voltArr[MODULE_COUNT*CELL_COUNT];
    uint8_t tempArr[MODULE_COUNT*CELL_COUNT];
    uint8_t balArr[MODULE_COUNT*CELL_COUNT];

    uint8_t page;
} BattLog;

void logEncode(Log *log, char *data);
bool logDecode(Log *log, char *data);

void battLogEncode(BattLog *log, char *data);
bool battLogDecode(BattLog *log, char *data);
#endif
