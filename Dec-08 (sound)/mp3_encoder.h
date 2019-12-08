#pragma once

#ifndef MP3_H
#define MP3_H

#include "stdbool.h"
#include "stdint.h"

#define PLAY_WITH_INDEX 0x03

void uart3_init(void);
void uart2_init(void);
bool sendDataToMP3(char cmd, uint8_t msb, uint8_t lsb);
bool uart2_get(char *input_byte);
bool uart2_put(char output_byte);
bool uart3_get(char *input_byte);
bool uart3_put(char output_byte);

void i2c2_init(void);

volatile int received;

#endif
