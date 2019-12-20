#include "alien_uart.h"
#include "gpio_driver.h"
#include "led_matrix.h"
#include "lpc40xx.h"
#include "mp3_encoder.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>

bool start_flag;
extern joystick joystick_control_signal;
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
uint8_t winner_matrix_red[16][64];
uint8_t winner_matrix_white[16][64];

void update_display_delay(void) {

  if (!start_flag) {
    for (int i = 0; i < 90; i++) {
      updateDisplay();
    }
  }
}

void startup_update_display_delay(void) {
  if (!start_flag) {
    for (int i = 0; i < 90; i++) {
      updateDisplay();
      (void)bluetooth_receive_data();

      if (joystick_control_signal == 9) {
        start_flag = true;
        i = 100;
        break;
      }
    }
  }
}

void level_up_screen(uint8_t level_number) {
  clearDisplay();
  updateDisplay();

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

void winner(void) {

  clearDisplay();
  updateDisplay();

  while (1) {
    for (int i = 0; i <= 15; i++) {
      for (int j = 0; j < 64; j++) {
        matrixbuff[i + 8][j] = winner_matrix_red[i][j];
      }
    }
    update_display_delay();
    for (int i = 0; i <= 15; i++) {
      for (int j = 0; j < 64; j++) {
        matrixbuff[i + 8][j] = winner_matrix_white[i][j];
      }
    }
    update_display_delay();
  }

#if 0

  while (1) {
    memcpy(matrixbuff, startup_new, (1 * 32 * 64));
    update_display_delay();
    clearDisplay();
    update_display_delay();
  }

#else

  memcpy(matrixbuff, game_over_red, (1 * 32 * 64));
  update_display_delay();

  while (joystick_control_signal != 9)
    ;

  configuration_init();

#endif
}

void game_over_screen(void) {

#if 1

  while (1) {
    memcpy(matrixbuff, game_over_red, (1 * 32 * 64));
    update_display_delay();
    clearDisplay();
    update_display_delay();
  }

#else

  memcpy(matrixbuff, game_over_red, (1 * 32 * 64));
  update_display_delay();

  while (joystick_control_signal != 9)
    ;

  configuration_init();

#endif
}

void startup_screen(void) {
  // LPC_GPIO0->DIR &= ~(1 << 29);

  while (1) {
    memcpy(matrixbuff, startup_new, (1 * 32 * 64));
    startup_update_display_delay();
    memcpy(matrixbuff, startup_new_two, (1 * 32 * 64));
    startup_update_display_delay();

    (void)bluetooth_receive_data();
    if ((joystick_control_signal == 9) || (start_flag == true)) {
      break;
    }
  }

  clearDisplay();
  updateDisplay();
  start_flag = false;
  joystick_control_signal = 0;

  // trigger to play are you ready!
  // PLAY_AREYOUREADY();
  play(AREYOUREADY);

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
  clearDisplay();
  updateDisplay();
  delay__ms(900);
}

void burst_animation(uint8_t x, uint8_t y, uint8_t color) {

  updatePixel(x + 2, y + 2, color);
  updateDisplay();
  updateDisplay();
  updateDisplay();
  updatePixel(x, y, color);
  updatePixel(x + 1, y, color);
  updatePixel(x, y + 1, color);
  updatePixel(x + 3, y, color);
  updatePixel(x + 4, y, color);
  updatePixel(x + 4, y + 1, color);
  updatePixel(x, y + 3, color);
  updatePixel(x, y + 4, color);
  updatePixel(x + 2, y + 4, color);
  updatePixel(x + 3, y + 4, color);
  updatePixel(x + 3, y + 4, color);
  updatePixel(x + 4, y + 3, color);
  updateDisplay();
  updateDisplay();
  updateDisplay();
}
