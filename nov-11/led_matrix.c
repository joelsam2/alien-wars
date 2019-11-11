#include "led_matrix.h"
#include "lpc40xx.h"
#include "stdlib.h"

void init_led_matrix() {

  // R1
  LPC_IOCON->P0_10 = 0;
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
  LPC_IOCON->P0_11 = 0;
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

  gpio_dir(0, 10, 0);
  gpio_dir(0, 1, 0);
  gpio_dir(0, 18, 0);
  gpio_dir(0, 15, 0);
  gpio_dir(2, 9, 0);
  gpio_dir(2, 7, 0);
  gpio_dir(0, 11, 0);
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
}

void led_matrix_control(int type, bool state) {

  if (type == R1) {
    if (state == true) {
      gpio_set(0, 10, 1);
    } else {
      gpio_set(0, 10, 0);
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
      gpio_set(0, 11, 1);
    } else {
      gpio_set(0, 11, 0);
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
