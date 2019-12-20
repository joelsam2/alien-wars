#pragma once

#ifndef MP3_H
#define MP3_H

#include "i2c.h"
#include "led_matrix.h"
#include "stdbool.h"

#define PLAY_WITH_INDEX 0x03

void uart3_init(void);
void uart2_init(void);
void sendDataToMP3(int cmd, int data);
bool uart2_get(char *input_byte);
bool uart2_put(char output_byte);
bool uart3_get(char *input_byte);
bool uart3_put(char output_byte);

void play(uint8_t track);

// #define PLAY_BACKGROUNDMZK() ({ i2c__write_single(2, 0x46, 0x09, 0x01); })

// #define PLAY_AREYOUREADY() ({ i2c__write_single(2, 0x46, 0x09, 0x02); })

// #define PLAY_BULLET() ({ i2c__write_single(2, 0x46, 0x09, 0x03); })

// #define PLAY_COLLISION() ({ i2c__write_single(2, 0x46, 0x09, 0x04); })

#define BACKGROUND 1
#define AREYOUREADY 2
#define BULLET 3
#define COLLISION 4

#endif
