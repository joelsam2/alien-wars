#include "gpio_driver.h"
#include "led_matrix.h"
#include "lpc40xx.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>

bool start_flag;
extern uint8_t level_two_red[15][64];
extern uint8_t level_two_white[15][64];
extern uint8_t level_three_red[15][64];
extern uint8_t level_three_white[15][64];
extern uint8_t startup_new_two[32][64];
extern uint8_t startup_new[32][64];
extern uint8_t are[8][64];
extern uint8_t you[8][64];
extern uint8_t ready[8][64];
extern uint8_t game_over_red[32][64];

void update_display_delay(void) {

  if (!start_flag) {
    for (int i = 0; i < 90; i++) {
      updateDisplay();

      if ((LPC_GPIO0->PIN & (1 << 29))) {
        start_flag = true;
        i = 100;
        break;
      }
    }
  }
}

void level_up_screen(uint8_t level_number) {
  clearDisplay();
  // updateDisplay();

  for (int i = 0; i < 2; i++) {
    if (level_number == 2) {
      for (int i = 0; i <= 14; i++) {
        for (int j = 0; j < 64; j++) {
          matrixbuff[i + 9][j] = level_two_red[i][j];
        }
      }

      update_display_delay();

      for (int i = 0; i <= 14; i++) {
        for (int j = 0; j < 64; j++) {
          matrixbuff[i + 9][j] = level_two_white[i][j];
        }
      }
      update_display_delay();

    } else if (level_number == 3) {
      for (int i = 0; i <= 14; i++) {
        for (int j = 0; j < 64; j++) {
          matrixbuff[i + 9][j] = level_three_red[i][j];
        }
      }

      update_display_delay();

      for (int i = 0; i <= 14; i++) {
        for (int j = 0; j < 64; j++) {
          matrixbuff[i + 9][j] = level_three_white[i][j];
        }
      }
      update_display_delay();
    }
  }
}

void game_over_screen(void) {

  while (1) {
    memcpy(matrixbuff, game_over_red, (1 * 32 * 64));
    update_display_delay();
    clearDisplay();
    update_display_delay();
  }
}

void startup_screen(void) {
  LPC_GPIO0->DIR &= ~(1 << 29);

  while (1) {
    memcpy(matrixbuff, startup_new, (1 * 32 * 64));
    update_display_delay();
    memcpy(matrixbuff, startup_new_two, (1 * 32 * 64));
    update_display_delay();
    if ((LPC_GPIO0->PIN & (1 << 29)) || (start_flag == true)) {
      while (gpio_get(0, 29))
        ;
      break;
    }
  }

  clearDisplay();
  updateDisplay();
  start_flag = false;

  // trigger to play are you ready!
  gpio_set(2, 0, 1);
  delay__ms(10);
  gpio_set(2, 0, 0);

  for (int i = 0; i <= 7; i++) {
    for (int j = 0; j < 64; j++) {
      matrixbuff[i + 11][j] = are[i][j];
    }
  }

  update_display_delay();

  for (int i = 0; i <= 7; i++) {
    for (int j = 0; j < 64; j++) {
      matrixbuff[i + 11][j] = you[i][j];
    }
  }
  update_display_delay();

  for (int i = 0; i <= 7; i++) {
    for (int j = 0; j < 64; j++) {
      matrixbuff[i + 11][j] = ready[i][j];
    }
  }
  update_display_delay();
  delay__ms(900);
}