#ifndef SUTIL_H
#define SUTIL_H

#include <stdint.h>
#include <stdbool.h>
// #include <avr/pgmspace.h>

void sutilWrite(char *string, uint8_t pos, char *data);
void sutilWritePgm(char *string, uint8_t pos, const char *data);
void sutilWriteUInt(char *string, uint8_t pos, uint32_t data, uint8_t n);
void sutilWriteInt(char *string, uint8_t pos, int32_t data, uint8_t n);
void sutilWriteUDecimal(char *string, uint8_t pos, uint32_t data, uint8_t n, uint8_t d);
void sutilWriteDecimal(char *string, uint8_t pos, int32_t data, uint8_t n, uint8_t d);
uint32_t sutilReadUInt(char *string, uint8_t pos, uint8_t n);
int16_t sutilReadDecimal16(char* string, uint8_t pos, uint8_t n, uint8_t* decimalPlaces, bool* valid);
int32_t sutilReadDecimal32(char *string, uint8_t pos, uint8_t n, uint8_t *decimalPlaces, bool *valid);

#endif
