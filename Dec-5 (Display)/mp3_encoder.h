#pragma once

#ifndef MP3_H
#define MP3_H

#include "stdbool.h"

#define PLAY_WITH_INDEX 0x03

void uart3_init(void);
void uart2_init(void);
void sendDataToMP3(int cmd, int data);
bool uart2_get(char *input_byte);
bool uart2_put(char output_byte);
bool uart3_get(char *input_byte);
bool uart3_put(char output_byte);

#define PLAY_BACKGROUNDMZK()                                                                                           \
  ({                                                                                                                   \
    gpio_set(2, 2, 1);                                                                                                 \
    delay__ms(10);                                                                                                     \
    gpio_set(2, 2, 0);                                                                                                 \
  })

#define PLAY_AREYOUREADY()                                                                                             \
  ({                                                                                                                   \
    gpio_set(2, 0, 1);                                                                                                 \
    delay__ms(10);                                                                                                     \
    gpio_set(2, 0, 0);                                                                                                 \
  })

#define PLAY_BULLET()                                                                                                  \
  ({                                                                                                                   \
    gpio_set(1, 28, 1);                                                                                                \
    delay__ms(10);                                                                                                     \
    gpio_set(1, 28, 0);                                                                                                \
  })

#define PLAY_COLLISION()                                                                                               \
  ({                                                                                                                   \
    gpio_set(1, 29, 1);                                                                                                \
    delay__ms(10);                                                                                                     \
    gpio_set(1, 29, 0);                                                                                                \
  })

#endif
