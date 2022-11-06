#ifndef MSG_H
#define MSG_H

#include <stdint.h>
#include <stdbool.h>
#include "mode.h"

#define MSG_RX 1
#define MSG_TX 2

typedef enum {
	MSG_BATT_STAT,
	MSG_BATT_VOLT,
	MSG_BATT_TEMP,
	MSG_BATT_CUR,
    MSG_BATT_SOC,
	MSG_CELL_STAT,
	MSG_DRIVE_STAT,
	MSG_PRESSURE_STAT,
	MSG_PERIPH_STAT,
	MSG_MOTOR_RESET,
	MSG_MOTOR_STAT,
	MSG_MOTOR_VEL,
	MSG_MOTOR_TEMP,
	MSG_MOTOR_CMD,
	MSG_BATT_CHARGE
} MsgType;

typedef struct {
	Mode mode;
	bool motorState;
	bool arrayState;
    bool systemsState;
	bool balState;
	uint8_t bfanState;
} MsgBattStat;

typedef struct {
	uint16_t voltSuppInt;
	uint16_t voltMin;
	uint16_t voltAv;
	uint16_t voltMax;
} MsgBattVolt;

typedef struct {
	uint8_t tempMin;
	uint8_t tempAv;
	uint8_t tempMax;
	uint8_t airTemp;
} MsgBattTemp;

typedef struct {
	int16_t curSystems;
	int16_t curBattery;
	int16_t curMotor;
	int16_t curArray;
} MsgBattCur;

typedef struct {
	uint8_t index;
	uint16_t cellVolt0;
	uint16_t cellVolt1;
	uint8_t cellTemp0;
	uint8_t cellTemp1;
	uint8_t cellBal0;
	uint8_t cellBal1;
} MsgCellStat;

typedef struct {
    int32_t soc;
} MsgBattSOC;

typedef struct {
	uint16_t velCruise;
	uint16_t pedal;
	uint16_t regen;
	bool wheelFlag;
	bool dirFlag;
	bool cruiseFlag;
} MsgDriveStat;

typedef struct {
	uint16_t pressureA;
	uint16_t pressureB;
	uint16_t pressureC;
	uint16_t pressureD;
} MsgPressureStat;

typedef struct {
	uint16_t voltSuppExt;
	bool brakeFlag;
	bool leftFlag;
	bool rightFlag;
	bool hazardFlag;
	bool hlightFlag;
	bool mfanFlag;
	bool dfanFlag;
} MsgPeriphStat;

typedef struct {
	int64_t resetCommand;
} MsgMotorReset;

typedef struct {
	uint8_t limitFlags;
	uint16_t errorFlags;
} MsgMotorStat;

typedef struct {
	float fVelocity;
} MsgMotorVel;

typedef struct {
	float fTempMotor;
} MsgMotorTemp;

typedef struct {
	float fVelDrive;
	float fCurDrive;
} MsgMotorCmd;

typedef struct {
	int16_t charge_cur;
} MsgBattCharge;

void msgEnable(MsgType msgType, uint8_t msgMode);
void msgInit(void);
bool msgAvailable(MsgType msgType);
void msgRead(MsgType msgType, void *msg);
void msgWrite(MsgType msgType, void *msg);

#endif
