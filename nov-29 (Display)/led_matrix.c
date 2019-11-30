#include "led_matrix.h"
#include "gpio_driver.h"
#include "lpc40xx.h"
#include "stdlib.h"
#include <string.h>

uint8_t firing_position_x, firing_position_y, ship_position_x, ship_position_y;

uint64_t data;
uint64_t spaceship_char[] = {0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000010000000000000000000000000000000000000000000,
                             0B0000000000000000000001000000000000000000000000000000000000000000,
                             0B0000000000000000000001100000000000000000000000000000000000000000,
                             0B0000000000000000000011111000000000000000000000000000000000000000,
                             0B0000000000000000000011111000000000000000000000000000000000000000,
                             0B0000000000000000000001100000000000000000000000000000000000000000,
                             0B0000000000000000000001000000000000000000000000000000000000000000,
                             0B0000000000000000000010000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000,
                             0B0000000000000000000000000000000000000000000000000000000000000000};

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

  // if ((x < 0) || (x > 32))
  //   return -1;
  // if ((y < 0) || (y > 64))
  //   return -1;

  matrixbuff[x][y] = color;

  return 0;
}

void drawSpaceship(uint8_t x, uint8_t y, uint8_t color) {

  updatePixel(x, y, color);
  position.spacecraft[0][0] = x;
  position.spacecraft[0][1] = y;

  updatePixel(x, y + 1, 0);
  position.spacecraft[1][0] = x;
  position.spacecraft[1][1] = y + 1;

  updatePixel(x, y + 2, 0);
  position.spacecraft[2][0] = x;
  position.spacecraft[2][1] = y + 2;

  updatePixel(x, y + 3, 0);
  position.spacecraft[3][0] = x;
  position.spacecraft[3][1] = y + 3;

  updatePixel(x, y + 4, 0);
  position.spacecraft[4][0] = x;
  position.spacecraft[4][1] = y + 4;

  updatePixel(x + 1, y, 0);
  position.spacecraft[5][0] = x + 1;
  position.spacecraft[5][1] = y;

  updatePixel(x + 1, y + 1, color);
  position.spacecraft[6][0] = x + 1;
  position.spacecraft[6][1] = y + 1;

  updatePixel(x + 1, y + 2, 0);
  position.spacecraft[7][0] = x + 1;
  position.spacecraft[7][1] = y + 2;

  updatePixel(x + 1, y + 3, 0);
  position.spacecraft[8][0] = x + 1;
  position.spacecraft[8][1] = y + 3;

  updatePixel(x + 1, y + 4, 0);
  position.spacecraft[9][0] = x + 1;
  position.spacecraft[9][1] = y + 4;

  updatePixel(x + 2, y, 0);
  position.spacecraft[10][0] = x + 2;
  position.spacecraft[10][1] = y;

  updatePixel(x + 2, y + 1, color);
  position.spacecraft[11][0] = x + 2;
  position.spacecraft[11][1] = y + 1;

  updatePixel(x + 2, y + 2, color);
  position.spacecraft[12][0] = x + 2;
  position.spacecraft[12][1] = y + 2;

  updatePixel(x + 2, y + 3, 0);
  position.spacecraft[13][0] = x + 2;
  position.spacecraft[13][1] = y + 3;

  updatePixel(x + 2, y + 4, 0);
  position.spacecraft[14][0] = x + 2;
  position.spacecraft[14][1] = y + 4;

  updatePixel(x + 3, y, color);
  position.spacecraft[15][0] = x + 3;
  position.spacecraft[15][1] = y;

  updatePixel(x + 3, y + 1, color);
  position.spacecraft[16][0] = x + 3;
  position.spacecraft[16][1] = y + 1;

  updatePixel(x + 3, y + 2, color);
  position.spacecraft[17][0] = x + 3;
  position.spacecraft[17][1] = y + 2;

  updatePixel(x + 3, y + 3, color);
  position.spacecraft[18][0] = x + 3;
  position.spacecraft[18][1] = y + 3;

  updatePixel(x + 3, y + 4, color);
  position.spacecraft[19][0] = x + 3;
  position.spacecraft[19][1] = y + 4;

  updatePixel(x + 4, y, color);
  position.spacecraft[20][0] = x + 4;
  position.spacecraft[20][1] = y;

  updatePixel(x + 4, y + 1, color);
  position.spacecraft[21][0] = x + 4;
  position.spacecraft[21][1] = y + 1;

  updatePixel(x + 4, y + 2, color);
  position.spacecraft[22][0] = x + 4;
  position.spacecraft[22][1] = y + 2;

  updatePixel(x + 4, y + 3, color);
  position.spacecraft[23][0] = x + 4;
  position.spacecraft[23][1] = y + 3;

  updatePixel(x + 4, y + 4, color);
  position.spacecraft[24][0] = x + 4;
  position.spacecraft[24][1] = y + 4;

  updatePixel(x + 5, y, 0);
  position.spacecraft[25][0] = x + 5;
  position.spacecraft[25][1] = y;

  updatePixel(x + 5, y + 1, color);
  position.spacecraft[26][0] = x + 5;
  position.spacecraft[26][1] = y + 1;

  updatePixel(x + 5, y + 2, color);
  position.spacecraft[27][0] = x + 5;
  position.spacecraft[27][1] = y + 2;

  updatePixel(x + 5, y + 3, 0);
  position.spacecraft[28][0] = x + 5;
  position.spacecraft[28][1] = y + 3;

  updatePixel(x + 5, y + 4, 0);
  position.spacecraft[29][0] = x + 5;
  position.spacecraft[29][1] = y + 4;

  updatePixel(x + 6, y, 0);
  position.spacecraft[30][0] = x + 6;
  position.spacecraft[30][1] = y;

  updatePixel(x + 6, y + 1, color);
  position.spacecraft[31][0] = x + 6;
  position.spacecraft[31][1] = y + 1;

  updatePixel(x + 6, y + 2, 0);
  position.spacecraft[32][0] = x + 6;
  position.spacecraft[32][1] = y + 2;

  updatePixel(x + 6, y + 3, 0);
  position.spacecraft[33][0] = x + 6;
  position.spacecraft[33][1] = y + 3;

  updatePixel(x + 6, y + 4, 0);
  position.spacecraft[34][0] = x + 6;
  position.spacecraft[34][1] = y + 4;

  updatePixel(x + 7, y, color);
  position.spacecraft[35][0] = x + 7;
  position.spacecraft[35][1] = y;

  updatePixel(x + 7, y + 1, 0);
  position.spacecraft[36][0] = x + 7;
  position.spacecraft[36][1] = y + 1;

  updatePixel(x + 7, y + 2, 0);
  position.spacecraft[37][0] = x + 7;
  position.spacecraft[37][1] = y + 2;

  updatePixel(x + 7, y + 3, 0);
  position.spacecraft[38][0] = x + 7;
  position.spacecraft[38][1] = y + 3;

  updatePixel(x + 7, y + 4, 0);
  position.spacecraft[39][0] = x + 7;
  position.spacecraft[39][1] = y = 4;
}

void drawEnemy(uint8_t x, uint8_t y, uint8_t color) {

  int pixel_count = 0;

  updatePixel(x, y, color);
  position.enemy[position.enemy_count][pixel_count][0] = x;
  position.enemy[position.enemy_count][pixel_count++][1] = y;

  updatePixel(x, y + 1, color);
  position.enemy[position.enemy_count][pixel_count][0] = x;
  position.enemy[position.enemy_count][pixel_count++][1] = y + 1;

  updatePixel(x, y + 2, color);
  position.enemy[position.enemy_count][pixel_count][0] = x;
  position.enemy[position.enemy_count][pixel_count++][1] = y + 2;

  updatePixel(x, y + 3, color);
  position.enemy[position.enemy_count][pixel_count][0] = x;
  position.enemy[position.enemy_count][pixel_count++][1] = y + 3;

  updatePixel(x, y + 4, color);
  position.enemy[position.enemy_count][pixel_count][0] = x;
  position.enemy[position.enemy_count][pixel_count++][1] = y + 4;

  updatePixel(x + 1, y - 1, color);
  position.enemy[position.enemy_count][pixel_count][0] = x + 1;
  position.enemy[position.enemy_count][pixel_count++][1] = y - 1;

  updatePixel(x + 1, y, color);
  position.enemy[position.enemy_count][pixel_count][0] = x + 1;
  position.enemy[position.enemy_count][pixel_count++][1] = y;

  updatePixel(x + 1, y + 1, 0);
  position.enemy[position.enemy_count][pixel_count][0] = x + 1;
  position.enemy[position.enemy_count][pixel_count++][1] = y + 1;

  updatePixel(x + 1, y + 2, 0);
  position.enemy[position.enemy_count][pixel_count][0] = x + 1;
  position.enemy[position.enemy_count][pixel_count++][1] = y + 2;

  updatePixel(x + 1, y + 3, 0);
  position.enemy[position.enemy_count][pixel_count][0] = x + 1;
  position.enemy[position.enemy_count][pixel_count++][1] = y + 3;

  updatePixel(x + 1, y + 4, color);
  position.enemy[position.enemy_count][pixel_count][0] = x + 1;
  position.enemy[position.enemy_count][pixel_count++][1] = y + 4;

  updatePixel(x + 2, y, color);
  position.enemy[position.enemy_count][pixel_count][0] = x + 2;
  position.enemy[position.enemy_count][pixel_count++][1] = y;

  updatePixel(x + 2, y + 1, color);
  position.enemy[position.enemy_count][pixel_count][0] = x + 2;
  position.enemy[position.enemy_count][pixel_count++][1] = y + 1;

  updatePixel(x + 2, y + 2, color);
  position.enemy[position.enemy_count][pixel_count][0] = x + 2;
  position.enemy[position.enemy_count][pixel_count++][1] = y + 2;

  updatePixel(x + 2, y + 3, color);
  position.enemy[position.enemy_count][pixel_count][0] = x + 2;
  position.enemy[position.enemy_count][pixel_count++][1] = y + 3;

  updatePixel(x + 2, y + 4, color);
  position.enemy[position.enemy_count][pixel_count][0] = x + 2;
  position.enemy[position.enemy_count][pixel_count++][1] = y + 4;

  // position.enemy_count++;
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

void clearDisplay(void) {
  for (uint8_t row = 0; row < 32; row++) {
    for (uint8_t col = 0; col < 64; col++) {
      matrixbuff[row][col] = 0;
    }
  }
}