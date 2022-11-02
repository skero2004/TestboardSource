#include "sutil.h"

static const uint32_t POWERS[] = {
	1, 10, 100, 1000, 10000, 100000, 1000000, 10000000,
};

void sutilWrite(char *string, uint8_t pos, char *data) {
	string += pos;
	while (*data != '\0') {
		*string = *data;
		string++;
		data++;
	}
}

void sutilWritePgm(char *string, uint8_t pos, const char *data) {
	string += pos;
	while (pgm_read_byte(data) != '\0') {
		*string = pgm_read_byte(data);
		string++;
		data++;
	}
}

void sutilWriteUInt(char *string, uint8_t pos, uint32_t data, uint8_t n) {
	for (uint8_t i = 0; i < n; i++) {
		string[pos+n-1-i] = data % 10 + '0';
		data /= 10;
	}
}

void sutilWriteInt(char *string, uint8_t pos, int32_t data, uint8_t n) {
	string[pos] = ((data >= 0) ? '+' : '-');
	sutilWriteUInt(string, pos+1, ((data >= 0) ? data : -data), n);
}

void sutilWriteUDecimal(char *string, uint8_t pos, uint32_t data, uint8_t n, uint8_t d) {
	string[pos+n-d] = '.';
	sutilWriteUInt(string, pos, data / POWERS[d], n-d);
	sutilWriteUInt(string, pos+n-d+1, data % POWERS[d], d);
}

void sutilWriteDecimal(char *string, uint8_t pos, int32_t data, uint8_t n, uint8_t d) {
	string[pos] = ((data >= 0) ? '+' : '-');
	sutilWriteUDecimal(string, pos+1, ((data >= 0) ? data : -data), n, d);
}

uint32_t sutilReadUInt(char *string, uint8_t pos, uint8_t n) {
	uint32_t data = 0;
	for (uint8_t i = 0; i < n; i++) {
		char c= string[pos+i];
		if (c < '0' || c > '9') return 0;
		data = 10*data + c - '0';
	}
	return data;
}

int16_t sutilReadDecimal16(char* string, uint8_t pos, uint8_t n, uint8_t* decimalPlaces, bool* valid) {
	*decimalPlaces = 0;
	*valid = false;

	int16_t data = 0;
	bool foundDecimal = false;
	bool negative = false;

	for (uint8_t i = 0; i < n; i++) {
		char c = string[pos + i];
		if (c == '-') {
			if (i == 0) {
				negative = true;
				continue;
			}

			return 0;
		}
		else if (c == '.') {
			if (!foundDecimal) {
				foundDecimal = true;
				*decimalPlaces = n - i - 1;
				continue;
			}

			return 0;
		}
		else {
			if (c < '0' || c > '9') {
				return 0;
			}
		}

		data = 10 * data + c - '0';
	}

	if (negative) data *= -1;

	*valid = true;
	return data;
}

int32_t sutilReadDecimal32(char *string, uint8_t pos, uint8_t n, uint8_t *decimalPlaces, bool *valid) {
	*decimalPlaces = 0;
	*valid = false;

	int32_t data = 0;
	bool foundDecimal = false;
	bool negative = false;

	for (uint8_t i = 0; i < n; i++) {
		char c = string[pos + i];
		if (c == '-') {
			if (i == 0) {
				negative = true;
				continue;
			}

			return 0;
		}
		else if (c == '.') {
			if (!foundDecimal) {
				foundDecimal = true;
				*decimalPlaces = n - i - 1;
				continue;
			}

			return 0;
		}
		else {
			if (c < '0' || c > '9') {
				return 0;
			}
		}

		data = 10 * data + c - '0';
	}

	if (negative) data *= -1;

	*valid = true;
	return data;
}