#include "led_matrix.h"
#include "gpio_driver.h"
#include "lpc40xx.h"
#include "stdlib.h"
#include <string.h>

void init_led_matrix() {

  // R1
  // LPC_IOCON->P0_10 = 0;
  LPC_IOCON->P2_5 = 0;
  // G1
  LPC_IOCON->P0_1 = 0;
  // B1
  LPC_IOCON->P0_18 = 0;
  // R2
  LPC_IOCON->P0_15 = 0;
  // G2
  LPC_IOCON->P2_9 = 0;
  // B2
  LPC_IOCON->P2_7 = 0;

  // A
  // LPC_IOCON->P0_11 = 0;
  LPC_IOCON->P2_4 = 0;
  // B
  LPC_IOCON->P0_0 = 0;
  // C
  LPC_IOCON->P0_22 = 0;
  // D
  LPC_IOCON->P0_17 = 0;

  // CLK
  LPC_IOCON->P0_16 = 0;
  // LAT
  LPC_IOCON->P2_8 = 0;
  // OE
  LPC_IOCON->P2_6 = 0;

  // gpio_dir(0, 10, 0);
  gpio_dir(2, 5, 0);
  gpio_dir(0, 1, 0);
  gpio_dir(0, 18, 0);
  gpio_dir(0, 15, 0);
  gpio_dir(2, 9, 0);
  gpio_dir(2, 7, 0);
  // gpio_dir(0, 11, 0);
  gpio_dir(2, 4, 0);
  gpio_dir(0, 0, 0);
  gpio_dir(0, 22, 0);
  gpio_dir(0, 17, 0);
  gpio_dir(0, 16, 0);
  gpio_dir(2, 8, 0);
  gpio_dir(2, 6, 0);

  led_matrix_control(OE, true);
  led_matrix_control(R1, false);
  led_matrix_control(G1, false);
  led_matrix_control(B1, false);
  led_matrix_control(R2, false);
  led_matrix_control(G2, false);
  led_matrix_control(B2, false);
  led_matrix_control(A, false);
  led_matrix_control(B, false);
  led_matrix_control(C, false);
  led_matrix_control(D, false);
  led_matrix_control(CLK, false);
  led_matrix_control(LAT, false);

  memset(matrixbuff, 0, 32 * 64);
}

void led_matrix_control(int type, bool state) {

  if (type == R1) {
    if (state == true) {
      // gpio_set(0, 10, 1);  //changed pins to accomodate uart
      gpio_set(2, 5, 1);

    } else {
      // gpio_set(0, 10, 0);  //changed pins to accomodate uart
      gpio_set(2, 5, 0);
    }
  } else if (type == G1) {
    if (state == true) {
      gpio_set(0, 1, 1);
    } else {
      gpio_set(0, 1, 0);
    }
  } else if (type == B1) {
    if (state == true) {
      gpio_set(0, 18, 1);
    } else {
      gpio_set(0, 18, 0);
    }
  } else if (type == R2) {
    if (state == true) {
      gpio_set(0, 15, 1);
    } else {
      gpio_set(0, 15, 0);
    }
  } else if (type == G2) {
    if (state == true) {
      gpio_set(2, 9, 1);
    } else {
      gpio_set(2, 9, 0);
    }
  } else if (type == B2) {
    if (state == true) {
      gpio_set(2, 7, 1);
    } else {
      gpio_set(2, 7, 0);
    }
  } else if (type == A) {
    if (state == true) {
      // gpio_set(0, 11, 1);
      gpio_set(2, 4, 1);
    } else {
      // gpio_set(0, 11, 0);
      gpio_set(2, 4, 0);
    }
  } else if (type == B) {
    if (state == true) {
      gpio_set(0, 0, 1);
    } else {
      gpio_set(0, 0, 0);
    }
  } else if (type == C) {
    if (state == true) {
      gpio_set(0, 22, 1);
    } else {
      gpio_set(0, 22, 0);
    }
  } else if (type == D) {
    if (state == true) {
      gpio_set(0, 17, 1);
    } else {
      gpio_set(0, 17, 0);
    }
  } else if (type == CLK) {
    if (state == true) {
      gpio_set(0, 16, 1);
    } else {
      gpio_set(0, 16, 0);
    }
  } else if (type == LAT) {
    if (state == true) {
      gpio_set(2, 8, 1);
    } else {
      gpio_set(2, 8, 0);
    }
  } else if (type == OE) {
    if (state == true) {
      gpio_set(2, 6, 1);
    } else {
      gpio_set(2, 6, 0);
    }
  }
}

void updateDisplay(void) {

  for (uint8_t row = 0; row < 32; row++) {

    for (uint8_t col = 0; col < 64; col++) {

      if (matrixbuff[row][col] & 0x1 && (row < 16)) {
        led_matrix_control(B1, true);
      } else {
        led_matrix_control(B1, false);
      }
      if (matrixbuff[row][col] & 0x2 && (row < 16)) {
        led_matrix_control(G1, true);
      } else {
        led_matrix_control(G1, false);
      }
      if (matrixbuff[row][col] & 0x4 && (row < 16)) {
        led_matrix_control(R1, true);
      } else {
        led_matrix_control(R1, false);
      }
      if (matrixbuff[row][col] & 0x1 && (row > 15)) {
        led_matrix_control(B2, true);
      } else {
        led_matrix_control(B2, false);
      }
      if (matrixbuff[row][col] & 0x2 && (row > 15)) {
        led_matrix_control(G2, true);
      } else {
        led_matrix_control(G2, false);
      }
      if (matrixbuff[row][col] & 0x4 && (row > 15)) {
        led_matrix_control(R2, true);
      } else {
        led_matrix_control(R2, false);
      }

      led_matrix_control(CLK, true);
      led_matrix_control(CLK, false);
    }

    led_matrix_control(OE, true);
    led_matrix_control(LAT, true);

    led_matrix_control(A, false);
    led_matrix_control(B, false);
    led_matrix_control(C, false);
    led_matrix_control(D, false);

    if (row & 0x1) {
      led_matrix_control(A, true);
    }
    if (row & 0x2) {
      led_matrix_control(B, true);
    }
    if (row & 0x4) {
      led_matrix_control(C, true);
    }
    if (row & 0x8) {
      led_matrix_control(D, true);
    }

    led_matrix_control(LAT, false);
    led_matrix_control(OE, false);
  }
}

int8_t updatePixel(uint8_t x, uint8_t y, uint8_t color) {

  matrixbuff[x][y] = color;

  return 0;
}

void clearDisplay(void) {

  for (uint8_t row = 0; row < 32; row++) {
    for (uint8_t col = 0; col < 64; col++) {
      matrixbuff[row][col] = 0;
    }
  }
}

void drawSpaceship(uint8_t x, uint8_t y, uint8_t color) {

  updatePixel(x, y, color);
  updatePixel(x, y + 1, 0);
  updatePixel(x, y + 2, 0);
  updatePixel(x, y + 3, 0);
  updatePixel(x, y + 4, 0);

  updatePixel(x + 1, y, 0);
  updatePixel(x + 1, y + 1, color);
  updatePixel(x + 1, y + 2, 0);
  updatePixel(x + 1, y + 3, 0);
  updatePixel(x + 1, y + 4, 0);

  updatePixel(x + 2, y, 0);
  updatePixel(x + 2, y + 1, color);
  updatePixel(x + 2, y + 2, color);
  updatePixel(x + 2, y + 3, 0);
  updatePixel(x + 2, y + 4, 0);

  updatePixel(x + 3, y, color);
  updatePixel(x + 3, y + 1, color);
  updatePixel(x + 3, y + 2, color);
  updatePixel(x + 3, y + 3, color);
  updatePixel(x + 3, y + 4, 0);

  updatePixel(x + 4, y, color);
  updatePixel(x + 4, y + 1, 0);
  updatePixel(x + 4, y + 2, 0);
  updatePixel(x + 4, y + 3, color);
  updatePixel(x + 4, y + 4, color);

  updatePixel(x + 5, y, color);
  updatePixel(x + 5, y + 1, color);
  updatePixel(x + 5, y + 2, color);
  updatePixel(x + 5, y + 3, color);
  updatePixel(x + 5, y + 4, 0);

  updatePixel(x + 6, y, 0);
  updatePixel(x + 6, y + 1, color);
  updatePixel(x + 6, y + 2, color);
  updatePixel(x + 6, y + 3, 0);
  updatePixel(x + 6, y + 4, 0);

  updatePixel(x + 7, y, 0);
  updatePixel(x + 7, y + 1, color);
  updatePixel(x + 7, y + 2, 0);
  updatePixel(x + 7, y + 3, 0);
  updatePixel(x + 7, y + 4, 0);

  updatePixel(x + 8, y, color);
  updatePixel(x + 8, y + 1, 0);
  updatePixel(x + 8, y + 2, 0);
  updatePixel(x + 8, y + 3, 0);
  updatePixel(x + 8, y + 4, 0);
}

void drawEnemy(uint8_t x, uint8_t y, uint8_t color) {

  int pixel_count = 0;

  updatePixel(x, y, color);
  updatePixel(x, y + 1, color);
  updatePixel(x, y + 2, color);
  updatePixel(x, y + 3, color);
  updatePixel(x, y + 4, color);

  updatePixel(x + 1, y - 1, color);
  updatePixel(x + 1, y, color);
  updatePixel(x + 1, y + 1, 0);
  updatePixel(x + 1, y + 2, 0);
  updatePixel(x + 1, y + 3, 0);
  updatePixel(x + 1, y + 4, color);

  updatePixel(x + 2, y, color);
  updatePixel(x + 2, y + 1, color);
  updatePixel(x + 2, y + 2, color);
  updatePixel(x + 2, y + 3, color);
  updatePixel(x + 2, y + 4, color);
}

#if 0
void firing_start_position(uint8_t x, uint8_t y, uint8_t color)
{

  updatePixel(x + 4, y, color);
  updatePixel(x + 4, y + 1, color);
  updatePixel(x + 4, y + 2, color);
  updatePixel(x + 4, y + 3, color);
  updatePixel(x + 4, y + 4, color);
  
}
#endif

void firing_position(void) {

  firing_position_x = ship_position_x + 4;
  firing_position_y = ship_position_y + 5;
}

void enemy1_firing_position(void) {

  enemy1_firing_position_x = x1 + 1;
  enemy1_firing_position_y = y1 - 1;
}

void drawVillain(uint8_t x, uint8_t y, uint8_t color) {

  updatePixel(x - 1, y + 1, color);
  updatePixel(x - 1, y + 2, 7);
  updatePixel(x - 1, y + 3, 7);
  updatePixel(x - 1, y + 4, 7);
  updatePixel(x - 1, y + 5, 7);
  updatePixel(x - 1, y + 6, 7);
  updatePixel(x - 1, y + 7, 7);
  updatePixel(x - 1, y + 8, 7);
  updatePixel(x - 1, y + 9, 7);
  updatePixel(x - 1, y + 10, 7);
  updatePixel(x - 1, y + 11, 7);
  updatePixel(x - 1, y + 12, color);
  updatePixel(x - 1, y + 13, 0);
  updatePixel(x - 1, y + 14, color);

  updatePixel(x - 2, y + 2, color);
  updatePixel(x - 2, y + 3, color);
  updatePixel(x - 2, y + 4, color);
  updatePixel(x - 2, y + 5, color);
  updatePixel(x - 2, y + 6, color);
  updatePixel(x - 2, y + 7, color);
  updatePixel(x - 2, y + 8, color);
  updatePixel(x - 2, y + 9, color);
  updatePixel(x - 2, y + 10, color);
  updatePixel(x - 2, y + 11, 7);
  updatePixel(x - 2, y + 12, color);
  updatePixel(x - 2, y + 13, 0);
  updatePixel(x - 2, y + 14, color);

  updatePixel(x - 3, y + 3, color);
  updatePixel(x - 3, y + 4, 0);
  updatePixel(x - 3, y + 5, 0);
  updatePixel(x - 3, y + 6, 0);
  updatePixel(x - 3, y + 7, 0);
  updatePixel(x - 3, y + 8, 0);
  updatePixel(x - 3, y + 9, 0);
  updatePixel(x - 3, y + 10, color);
  updatePixel(x - 3, y + 11, 7);
  updatePixel(x - 3, y + 12, color);
  updatePixel(x - 3, y + 13, 0);
  updatePixel(x - 3, y + 14, color);

  updatePixel(x - 4, y + 4, color);
  updatePixel(x - 4, y + 5, 0);
  updatePixel(x - 4, y + 6, 0);
  updatePixel(x - 4, y + 7, 0);
  updatePixel(x - 4, y + 8, 0);
  updatePixel(x - 4, y + 9, 0);
  updatePixel(x - 4, y + 10, color);
  updatePixel(x - 4, y + 11, 7);
  updatePixel(x - 4, y + 12, color);
  updatePixel(x - 4, y + 13, color);

  updatePixel(x - 5, y + 2, color);
  updatePixel(x - 5, y + 3, color);
  updatePixel(x - 5, y + 4, color);
  updatePixel(x - 5, y + 5, color);
  updatePixel(x - 5, y + 6, color);
  updatePixel(x - 5, y + 7, color);
  updatePixel(x - 5, y + 8, color);
  updatePixel(x - 5, y + 9, 0);
  updatePixel(x - 5, y + 10, color);
  updatePixel(x - 5, y + 11, 7);
  updatePixel(x - 5, y + 12, color);

  updatePixel(x - 6, y + 3, color);
  updatePixel(x - 6, y + 4, 0);
  updatePixel(x - 6, y + 5, 0);
  updatePixel(x - 6, y + 6, 0);
  updatePixel(x - 6, y + 7, 0);
  updatePixel(x - 6, y + 8, color);
  updatePixel(x - 6, y + 9, 0);
  updatePixel(x - 6, y + 10, color);
  updatePixel(x - 6, y + 11, color);

  updatePixel(x - 7, y + 4, color);
  updatePixel(x - 7, y + 5, 0);
  updatePixel(x - 7, y + 6, 0);
  updatePixel(x - 7, y + 7, 0);
  updatePixel(x - 7, y + 8, color);
  updatePixel(x - 7, y + 9, 0);
  updatePixel(x - 7, y + 10, color);

  updatePixel(x - 8, y + 5, color);
  updatePixel(x - 8, y + 6, 0);
  updatePixel(x - 8, y + 7, 0);
  updatePixel(x - 8, y + 8, color);
  updatePixel(x - 8, y + 9, 0);
  updatePixel(x - 8, y + 10, color);

  updatePixel(x - 9, y, color);
  updatePixel(x - 9, y + 1, color);
  updatePixel(x - 9, y + 2, color);
  updatePixel(x - 9, y + 3, color);
  updatePixel(x - 9, y + 4, color);
  updatePixel(x - 9, y + 5, color);
  updatePixel(x - 9, y + 6, color);
  updatePixel(x - 9, y + 7, 0);
  updatePixel(x - 9, y + 8, 0);
  updatePixel(x - 9, y + 9, color);
  updatePixel(x - 9, y + 10, color);

  updatePixel(x - 10, y + 7, color);
  updatePixel(x - 10, y + 8, 0);
  updatePixel(x - 10, y + 9, 0);
  updatePixel(x - 10, y + 10, color);

  updatePixel(x - 11, y + 8, color);
  updatePixel(x - 11, y + 9, 0);
  updatePixel(x - 11, y + 10, color);

  updatePixel(x - 12, y + 9, color);
  updatePixel(x - 12, y + 10, color);

  updatePixel(x - 13, y + 10, color);

  updatePixel(x, y, color);
  updatePixel(x, y + 1, 7);
  updatePixel(x, y + 2, 7);
  updatePixel(x, y + 3, 7);
  updatePixel(x, y + 4, 7);
  updatePixel(x, y + 5, 7);
  updatePixel(x, y + 6, 7);
  updatePixel(x, y + 7, 7);
  updatePixel(x, y + 8, 7);
  updatePixel(x, y + 9, 7);
  updatePixel(x, y + 10, 7);
  updatePixel(x, y + 11, 7);
  updatePixel(x, y + 12, color);
  updatePixel(x, y + 13, 0);
  updatePixel(x, y + 14, color);

  updatePixel(x + 1, y + 1, color);
  updatePixel(x + 1, y + 2, 7);
  updatePixel(x + 1, y + 3, 7);
  updatePixel(x + 1, y + 4, 7);
  updatePixel(x + 1, y + 5, 7);
  updatePixel(x + 1, y + 6, 7);
  updatePixel(x + 1, y + 7, 7);
  updatePixel(x + 1, y + 8, 7);
  updatePixel(x + 1, y + 9, 7);
  updatePixel(x + 1, y + 10, 7);
  updatePixel(x + 1, y + 11, 7);
  updatePixel(x + 1, y + 12, color);
  updatePixel(x + 1, y + 13, 0);
  updatePixel(x + 1, y + 14, color);

  updatePixel(x + 2, y + 2, color);
  updatePixel(x + 2, y + 3, color);
  updatePixel(x + 2, y + 4, color);
  updatePixel(x + 2, y + 5, color);
  updatePixel(x + 2, y + 6, color);
  updatePixel(x + 2, y + 7, color);
  updatePixel(x + 2, y + 8, color);
  updatePixel(x + 2, y + 9, color);
  updatePixel(x + 2, y + 10, color);
  updatePixel(x + 2, y + 11, 7);
  updatePixel(x + 2, y + 12, color);
  updatePixel(x + 2, y + 13, 0);
  updatePixel(x + 2, y + 14, color);

  updatePixel(x + 3, y + 3, color);
  updatePixel(x + 3, y + 4, 0);
  updatePixel(x + 3, y + 5, 0);
  updatePixel(x + 3, y + 6, 0);
  updatePixel(x + 3, y + 7, 0);
  updatePixel(x + 3, y + 8, 0);
  updatePixel(x + 3, y + 9, 0);
  updatePixel(x + 3, y + 10, color);
  updatePixel(x + 3, y + 11, 7);
  updatePixel(x + 3, y + 12, color);
  updatePixel(x + 3, y + 13, 0);
  updatePixel(x + 3, y + 14, color);

  updatePixel(x + 4, y + 4, color);
  updatePixel(x + 4, y + 5, 0);
  updatePixel(x + 4, y + 6, 0);
  updatePixel(x + 4, y + 7, 0);
  updatePixel(x + 4, y + 8, 0);
  updatePixel(x + 4, y + 9, 0);
  updatePixel(x + 4, y + 10, color);
  updatePixel(x + 4, y + 11, 7);
  updatePixel(x + 4, y + 12, color);
  updatePixel(x + 4, y + 13, color);

  updatePixel(x + 5, y + 2, color);
  updatePixel(x + 5, y + 3, color);
  updatePixel(x + 5, y + 4, color);
  updatePixel(x + 5, y + 5, color);
  updatePixel(x + 5, y + 6, color);
  updatePixel(x + 5, y + 7, color);
  updatePixel(x + 5, y + 8, color);
  updatePixel(x + 5, y + 9, 0);
  updatePixel(x + 5, y + 10, color);
  updatePixel(x + 5, y + 11, 7);
  updatePixel(x + 5, y + 12, color);

  updatePixel(x + 6, y + 3, color);
  updatePixel(x + 6, y + 4, 0);
  updatePixel(x + 6, y + 5, 0);
  updatePixel(x + 6, y + 6, 0);
  updatePixel(x + 6, y + 7, 0);
  updatePixel(x + 6, y + 8, color);
  updatePixel(x + 6, y + 9, 0);
  updatePixel(x + 6, y + 10, color);
  updatePixel(x + 6, y + 11, color);

  updatePixel(x + 7, y + 4, color);
  updatePixel(x + 7, y + 5, 0);
  updatePixel(x + 7, y + 6, 0);
  updatePixel(x + 7, y + 7, 0);
  updatePixel(x + 7, y + 8, color);
  updatePixel(x + 7, y + 9, 0);
  updatePixel(x + 7, y + 10, color);

  updatePixel(x + 8, y + 5, color);
  updatePixel(x + 8, y + 6, 0);
  updatePixel(x + 8, y + 7, 0);
  updatePixel(x + 8, y + 8, color);
  updatePixel(x + 8, y + 9, 0);
  updatePixel(x + 8, y + 10, color);

  updatePixel(x + 9, y, color);
  updatePixel(x + 9, y + 1, color);
  updatePixel(x + 9, y + 2, color);
  updatePixel(x + 9, y + 3, color);
  updatePixel(x + 9, y + 4, color);
  updatePixel(x + 9, y + 5, color);
  updatePixel(x + 9, y + 6, color);
  updatePixel(x + 9, y + 7, 0);
  updatePixel(x + 9, y + 8, 0);
  updatePixel(x + 9, y + 9, color);
  updatePixel(x + 9, y + 10, color);

  updatePixel(x + 10, y + 7, color);
  updatePixel(x + 10, y + 8, 0);
  updatePixel(x + 10, y + 9, 0);
  updatePixel(x + 10, y + 10, color);

  updatePixel(x + 11, y + 8, color);
  updatePixel(x + 11, y + 9, 0);
  updatePixel(x + 11, y + 10, color);

  updatePixel(x + 12, y + 9, color);
  updatePixel(x + 12, y + 10, color);

  updatePixel(x + 13, y + 10, color);
}

void drawEnemy_new(uint8_t x, uint8_t y, uint8_t color) {

  updatePixel(x, y, 0);
  updatePixel(x, y + 1, 0);
  updatePixel(x, y + 2, 0);
  updatePixel(x, y + 3, color);
  updatePixel(x, y + 4, 0);
  updatePixel(x, y + 5, 0);
  updatePixel(x, y + 6, 0);

  updatePixel(x + 1, y, 0);
  updatePixel(x + 1, y + 1, 0);
  updatePixel(x + 1, y + 2, color);
  updatePixel(x + 1, y + 3, color);
  updatePixel(x + 1, y + 4, 0);
  updatePixel(x + 1, y + 5, color);
  updatePixel(x + 1, y + 6, color);

  updatePixel(x + 2, y, 0);
  updatePixel(x + 2, y + 1, color);
  updatePixel(x + 2, y + 2, 0);
  updatePixel(x + 2, y + 3, color);
  updatePixel(x + 2, y + 4, color);
  updatePixel(x + 2, y + 5, 0);
  updatePixel(x + 2, y + 6, 0);

  updatePixel(x + 3, y, color);
  updatePixel(x + 3, y + 1, color);
  updatePixel(x + 3, y + 2, 0);
  updatePixel(x + 3, y + 3, 0);
  updatePixel(x + 3, y + 4, color);
  updatePixel(x + 3, y + 5, color);
  updatePixel(x + 3, y + 6, color);

  updatePixel(x + 4, y, 0);
  updatePixel(x + 4, y + 1, color);
  updatePixel(x + 4, y + 2, 0);
  updatePixel(x + 4, y + 3, color);
  updatePixel(x + 4, y + 4, color);
  updatePixel(x + 4, y + 5, 0);
  updatePixel(x + 4, y + 6, 0);

  updatePixel(x + 5, y, 0);
  updatePixel(x + 5, y + 1, 0);
  updatePixel(x + 5, y + 2, color);
  updatePixel(x + 5, y + 3, color);
  updatePixel(x + 5, y + 4, 0);
  updatePixel(x + 5, y + 5, color);
  updatePixel(x + 5, y + 6, color);

  updatePixel(x + 6, y, 0);
  updatePixel(x + 6, y + 1, 0);
  updatePixel(x + 6, y + 2, 0);
  updatePixel(x + 6, y + 3, color);
  updatePixel(x + 6, y + 4, 0);
  updatePixel(x + 6, y + 5, 0);
  updatePixel(x + 6, y + 6, 0);
}

// void burst_animation(uint8_t x, uint8_t y, uint8_t color) {

//   updatePixel(x, y, 0);
//   updatePixel(x, y + 1, color);
//   updatePixel(x, y + 2, color);
//   updatePixel(x, y + 3, color);
//   updatePixel(x, y + 4, 0);

//   updatePixel(x + 1, y, color);
//   updatePixel(x + 1, y + 1, 0);
//   updatePixel(x + 1, y + 2, color);
//   updatePixel(x + 1, y + 3, 0);
//   updatePixel(x + 1, y + 4, color);

//   updatePixel(x + 2, y, 0);
//   updatePixel(x + 2, y + 1, color);
//   updatePixel(x + 2, y + 2, color);
//   updatePixel(x + 2, y + 3, color);
//   updatePixel(x + 2, y + 4, 0);
// }