#include "can.h"
#include "msg.h"

static uint8_t data[8];

static void writeBool(bool x, uint8_t pos, uint8_t count);
static void writeByte(uint8_t x, uint8_t pos);
static void writeWord(uint16_t x, uint8_t pos);
static void writeInt32(int32_t x, uint8_t pos);
static void writeInt64(int64_t x, uint8_t pos);
static void writeFloat(float x, uint8_t pos);

static bool readBool(uint8_t pos, uint8_t count);
static uint8_t readByte(uint8_t pos);
static uint16_t readWord(uint8_t pos);
static int32_t readInt32(uint8_t pos);
static int64_t readInt64(uint8_t pos);
static float readFloat(uint8_t pos);

void msgEnable(MsgType msgType, uint8_t msgMode) {
	switch (msgType) {
		case MSG_BATT_STAT:
			canSetConfig(MSG_BATT_STAT, msgMode, 0x0100, 3);
			break;
		case MSG_BATT_VOLT:
			canSetConfig(MSG_BATT_VOLT, msgMode, 0x0101, 8);
			break;
		case MSG_BATT_TEMP:
			canSetConfig(MSG_BATT_TEMP, msgMode, 0x0102, 4);
			break;
		case MSG_BATT_CUR:
			canSetConfig(MSG_BATT_CUR, msgMode, 0x0103, 8);
			break;
		case MSG_BATT_SOC:
			canSetConfig(MSG_BATT_SOC, msgMode, 0x0104, 4);
			break;
		case MSG_CELL_STAT:
			canSetConfig(MSG_CELL_STAT, msgMode, 0x0105, 8);
			break;

		case MSG_DRIVE_STAT:
			canSetConfig(MSG_DRIVE_STAT, msgMode, 0x0200, 7);
			break;
		case MSG_PRESSURE_STAT:
			canSetConfig(MSG_PRESSURE_STAT, msgMode, 0x0201, 8);
			break;
		case MSG_PERIPH_STAT:
			canSetConfig(MSG_PERIPH_STAT, msgMode, 0x0202, 3);
			break;
		case MSG_MOTOR_RESET:
			canSetConfig(MSG_MOTOR_RESET, msgMode, 0x0203, 8);
			break;

		case MSG_MOTOR_STAT:
			canSetConfig(MSG_MOTOR_STAT, msgMode, 0x0401, 8);
			break;
		case MSG_MOTOR_VEL:
			canSetConfig(MSG_MOTOR_VEL, msgMode, 0x0403, 8);
			break;
		case MSG_MOTOR_TEMP:
			canSetConfig(MSG_MOTOR_TEMP, msgMode, 0x040B, 8);
			break;
		case MSG_MOTOR_CMD:
			canSetConfig(MSG_MOTOR_CMD, msgMode, 0x0501, 8);
			break;

		case MSG_BATT_CHARGE:
			canSetConfig(MSG_BATT_CHARGE, msgMode, 0x1806E5F4, 8);
			//identifier: identifier to help the receiving computer determine whether it should read the msg or not (idenfitifer is specific to the receiver; we should find this in the CAN charger documentation)
			//number_of_bytes: "how long is our message (in bytes)? - CAN Messages Slides"
			break;

		case MSG_CHARGE_STAT:
			canSetConfig(MSG_CHARGE_STAT, msgMode, 0x18FF50E5, 8);
			//identifier: identifier to help the receiving computer determine whether it should read the msg or not (idenfitifer is specific to the receiver; we should find this in the CAN charger documentation)
			//number_of_bytes: "how long is our message (in bytes)? - CAN Messages Slides"
			break;
	}
}

void msgInit(void) {
	canInit();
}

bool msgAvailable(MsgType msgType) {
	return canAvailable(msgType);
}

void msgRead(MsgType msgType, void *msg) {
	canRead(msgType, data);

	switch (msgType) {
		case MSG_BATT_STAT: {
			MsgBattStat* m = (MsgBattStat*) msg;
			m->mode = readByte(0);
			m->motorState = readBool(1, 0);
			m->arrayState = readBool(1, 1);
			m->systemsState = readBool(1, 2);
			m->balState = readBool(1, 3);
			m->bfanState = readByte(2);
			break;
		}
		case MSG_BATT_VOLT: {
			MsgBattVolt* m = (MsgBattVolt*) msg;
			m->voltSuppInt = readWord(0);
			m->voltMin = readWord(2);
			m->voltAv = readWord(4);
			m->voltMax = readWord(6);
			break;
		}
		case MSG_BATT_TEMP: {
			MsgBattTemp* m = (MsgBattTemp*) msg;
			m->tempMin = readByte(0);
			m->tempAv = readByte(1);
			m->tempMax = readByte(2);
			m->airTemp = readByte(3);
			break;
		}
		case MSG_BATT_CUR: {
			MsgBattCur* m = (MsgBattCur*) msg;
			m->curSystems = readWord(0);
			m->curBattery = readWord(2);
			m->curMotor = readWord(4);
			m->curArray = readWord(6);
			break;
		}
        case MSG_BATT_SOC: {
            MsgBattSOC* m = (MsgBattSOC*) msg;
            m->soc = readInt32(0);
            break;
        }
		case MSG_CELL_STAT: {
			MsgCellStat* m = (MsgCellStat*) msg;
			m->index = readByte(0);
			m->cellVolt0 = readWord(1);
			m->cellVolt1 = readWord(3);
			m->cellTemp0 = readByte(5);
			m->cellTemp1 = readByte(6);
			m->cellBal0 = readByte(7) & 0x0F;
			m->cellBal1 = readByte(7) >> 4;
			break;
		}

		case MSG_DRIVE_STAT: {
			MsgDriveStat* m = (MsgDriveStat*) msg;
			m->velCruise = readWord(0);
			m->pedal = readWord(2);
			m->regen = readWord(4);
			m->wheelFlag = readBool(6, 0);
			m->dirFlag = readBool(6, 1);
			m->cruiseFlag = readBool(6, 2);
			break;
		}
		case MSG_PRESSURE_STAT: {
			MsgPressureStat* m = (MsgPressureStat*) msg;
			m->pressureA = readWord(0);
			m->pressureB = readWord(2);
			m->pressureC = readWord(4);
			m->pressureD = readWord(6);
			break;
		}
		case MSG_PERIPH_STAT: {
			MsgPeriphStat* m = (MsgPeriphStat*) msg;
			m->voltSuppExt = readWord(0);
			m->brakeFlag = readBool(2, 0);
			m->leftFlag = readBool(2, 1);
			m->rightFlag = readBool(2, 2);
			m->hazardFlag = readBool(2, 3);
			m->hlightFlag = readBool(2, 4);
			m->mfanFlag = readBool(2, 5);
			m->dfanFlag = readBool(2, 6);
			break;
		}

		case MSG_MOTOR_RESET: {
			MsgMotorReset* m = (MsgMotorReset*)msg;
			m->resetCommand = readInt64(0);
			break;
		}

		case MSG_MOTOR_STAT: {
			MsgMotorStat* m = (MsgMotorStat*) msg;
			m->limitFlags = readByte(0);
			m->errorFlags = readWord(2);
			break;
		}
		case MSG_MOTOR_VEL: {
			MsgMotorVel* m = (MsgMotorVel*) msg;
			m->fVelocity = readFloat(0);
			break;
		}
		case MSG_MOTOR_TEMP: {
			MsgMotorTemp* m = (MsgMotorTemp*) msg;
			m->fTempMotor = readFloat(4);
			break;
		}
		case MSG_MOTOR_CMD: {
			MsgMotorCmd* m = (MsgMotorCmd*) msg;
			m->fVelDrive = readFloat(0);
			m->fCurDrive = readFloat(4);
			break;
		}
		case MSG_BATT_CHARGE: {
			MsgBattCharge* m = (MsgBattCharge*) msg;
			m->charge_voltage = readByte(0);
			m->charge_cur = readByte(2);
			break;
		}
		case MSG_CHARGE_STAT: {
			MsgChargeStat* m = (MsgChargeStat*) msg;
			m->charge_voltage = readByte(0);
			m->charge_cur = readByte(2);
			m->charge_status_flags = readByte(4);
			break;
		}
	}
}

void msgWrite(MsgType msgType, void *msg) {
	for (uint8_t i = 0; i < 8; i++) {
		data[i] = 0;
	}

	switch (msgType) {
		case MSG_BATT_STAT: {
			MsgBattStat* m = (MsgBattStat*) msg;
			writeByte(m->mode, 0);
			writeBool(m->motorState, 1, 0);
			writeBool(m->arrayState, 1, 1);
			writeBool(m->systemsState, 1, 2);
			writeBool(m->balState, 1, 3);
			writeByte(m->bfanState, 2);
			break;
		}
		case MSG_BATT_VOLT: {
			MsgBattVolt* m = (MsgBattVolt*) msg;
			writeWord(m->voltSuppInt, 0);
			writeWord(m->voltMin, 2);
			writeWord(m->voltAv, 4);
			writeWord(m->voltMax, 6);
			break;
		}
		case MSG_BATT_TEMP: {
			MsgBattTemp* m = (MsgBattTemp*) msg;
			writeByte(m->tempMin, 0);
			writeByte(m->tempAv, 1);
			writeByte(m->tempMax, 2);
			writeByte(m->airTemp, 3);
			break;
		}
		case MSG_BATT_CUR: {
			MsgBattCur* m = (MsgBattCur*) msg;
			writeWord(m->curSystems, 0);
			writeWord(m->curBattery, 2);
			writeWord(m->curMotor, 4);
			writeWord(m->curArray, 6);
			break;
		}
        case MSG_BATT_SOC: {
            MsgBattSOC* m = (MsgBattSOC*) msg;
            writeInt32(m->soc, 0);
            break;
        }
		case MSG_CELL_STAT: {
			MsgCellStat* m = (MsgCellStat*) msg;
			writeByte(m->index, 0);
			writeWord(m->cellVolt0, 1);
			writeWord(m->cellVolt1, 3);
			writeByte(m->cellTemp0, 5);
			writeByte(m->cellTemp1, 6);
			writeByte((m->cellBal1 << 4) | (m->cellBal0 & 0x0F), 7);
			break;
		}

		case MSG_DRIVE_STAT: {
			MsgDriveStat* m = (MsgDriveStat*) msg;
			writeWord(m->velCruise, 0);
			writeWord(m->pedal, 2);
			writeWord(m->regen, 4);
			writeBool(m->wheelFlag, 6, 0);
			writeBool(m->dirFlag, 6, 1);
			writeBool(m->cruiseFlag, 6, 2);
			break;
		}
		case MSG_PRESSURE_STAT: {
			MsgPressureStat* m = (MsgPressureStat*) msg;
			writeWord(m->pressureA, 0);
			writeWord(m->pressureB, 2);
			writeWord(m->pressureC, 4);
			writeWord(m->pressureD, 6);
			break;
		}
		case MSG_PERIPH_STAT: {
			MsgPeriphStat* m = (MsgPeriphStat*) msg;
			writeWord(m->voltSuppExt, 0);
			writeBool(m->brakeFlag, 2, 0);
			writeBool(m->leftFlag, 2, 1);
			writeBool(m->rightFlag, 2, 2);
			writeBool(m->hazardFlag, 2, 3);
			writeBool(m->hlightFlag, 2, 4);
			writeBool(m->mfanFlag, 2, 5);
			writeBool(m->dfanFlag, 2, 6);
			break;
		}
		case MSG_MOTOR_RESET: {
			MsgMotorReset* m = (MsgMotorReset*) msg;
			writeInt64(m->resetCommand, 0);
			break;
		}
		case MSG_MOTOR_STAT: {
			MsgMotorStat* m = (MsgMotorStat*) msg;
			writeByte(m->limitFlags, 0);
			writeWord(m->errorFlags, 2);
			break;
		}
		case MSG_MOTOR_VEL: {
			MsgMotorVel* m = (MsgMotorVel*) msg;
			writeFloat(m->fVelocity, 0);
			break;
		}
		case MSG_MOTOR_TEMP: {
			MsgMotorTemp* m = (MsgMotorTemp*) msg;
			writeFloat(m->fTempMotor, 4);
			break;
		}
		case MSG_MOTOR_CMD: {
			MsgMotorCmd* m = (MsgMotorCmd*) msg;
			writeFloat(m->fVelDrive, 0);
			writeFloat(m->fCurDrive, 4);
			break;
		}
		case MSG_BATT_CHARGE: {
			//msgBattCharge is a struct
			//msg is the pointer to the message object passed into this function
			MsgBattCharge* m = (MsgBattCharge*) msg;
			//This is what the message needs to look like: https://onedrive.live.com/?authkey=%21AKocUPTa4Amu0b8&cid=49EB4B401F0CD666&id=49EB4B401F0CD666%2189777&parId=49EB4B401F0CD666%2189775&o=OneUp
			writeByte(m->charge_voltage, 0);
			writeByte(m->charge_voltage, 1);
			writeByte(m->charge_cur, 2);
			writeByte(m->charge_cur, 3);
			writeByte(0, 4);
			writeByte(0, 5);
			writeByte(0, 6);
			writeByte(0, 7);
			break;
		}
		case MSG_CHARGE_STAT: {
			MsgChargeStat* m = (MsgChargeStat*) msg;
			writeByte(m->charge_voltage, 0);
			writeByte(m->charge_voltage, 1);
			writeByte(m->charge_cur, 2);
			writeByte(m->charge_cur, 3);
			writeByte(m->charge_status_flags, 4);
			writeByte(0, 5);
			writeByte(0, 6);
			writeByte(0, 7);
			break;
		}
	}

	canWrite(msgType, data);
}

static void writeBool(bool x, uint8_t pos, uint8_t count) {
	if (x) {
		data[pos] |= (1 << count);
	}
}

static void writeByte(uint8_t x, uint8_t pos) {
	data[pos] = x;
}

static void writeWord(uint16_t x, uint8_t pos) {
	uint8_t *ptr = (uint8_t*) &x;
	data[pos] = ptr[0];
	data[pos+1] = ptr[1];
}

static void writeInt32(int32_t x, uint8_t pos) {
	uint8_t *ptr = (uint8_t*) &x;
	data[pos] = ptr[0];
	data[pos+1] = ptr[1];
	data[pos+2] = ptr[2];
	data[pos+3] = ptr[3];
}

static void writeInt64(int64_t x, uint8_t pos) {
	uint8_t* ptr = (uint8_t*)& x;
	data[pos] = ptr[0];
	data[pos + 1] = ptr[1];
	data[pos + 2] = ptr[2];
	data[pos + 3] = ptr[3];
	data[pos + 4] = ptr[4];
	data[pos + 5] = ptr[5];
	data[pos + 6] = ptr[6];
	data[pos + 7] = ptr[7];
}

static void writeFloat(float x, uint8_t pos) {
	uint8_t *ptr = (uint8_t*) &x;
	data[pos] = ptr[0];
	data[pos+1] = ptr[1];
	data[pos+2] = ptr[2];
	data[pos+3] = ptr[3];
}

static bool readBool(uint8_t pos, uint8_t count) {
	if (data[pos] & (1 << count)) {
		return true;
	} else {
		return false;
	}
}

static uint8_t readByte(uint8_t pos) {
	return data[pos];
}

static uint16_t readWord(uint8_t pos) {
	return *((uint16_t*) &data[pos]);
}

static int32_t readInt32(uint8_t pos) {
	return *((int32_t*) &data[pos]);
}

static int64_t readInt64(uint8_t pos) {
	return *((int64_t*)& data[pos]);
}

static float readFloat(uint8_t pos) {
	return *((float*) &data[pos]);
}
