#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "gpio.h"
#include "sj2_cli.h"

#include "gpio_driver.h"
#include "led_matrix.h"

#include "lpc_peripherals.h"

#include "mp3_encoder.h"
#include "portmacro.h"
#include "semphr.h"
#include "uart.h"
#include <string.h>

extern int i2c_data;

int main(void) {

  // It is advised to either run the uart_task, or the SJ2 command-line (CLI), but not both
  // Change '#if 0' to '#if 1' and vice versa to try it out
#if 0
  // printf() takes more stack space, size this tasks' stack higher
  xTaskCreate(uart_task, "uart", (512U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
#else
  sj2_cli__init();
  // UNUSED(uart_task); // uart_task is un-used in if we are doing cli init()
#endif

  uint8_t state = 0;

  bool a, b, c, d;

  // shoot = xSemaphoreCreateBinary();

  init_led_matrix();

  // led
  LPC_IOCON->P2_3 = 0;
  gpio_dir(2, 3, 0);

  // buttons
  LPC_IOCON->P0_29 = 0;
  gpio_dir(0, 29, 1);

  LPC_IOCON->P0_30 = 0;
  gpio_dir(0, 30, 1);

  LPC_IOCON->P1_15 = 0;
  gpio_dir(1, 15, 1);

  LPC_IOCON->P1_19 = 0;
  gpio_dir(1, 19, 1);

  // mp3
  gpio_dir(2, 2, 1);
  LPC_IOCON->P2_2 |= 0x00;

  gpio_dir(2, 0, 1);
  LPC_IOCON->P2_0 |= 0x00;

  gpio_dir(1, 28, 1);
  LPC_IOCON->P1_28 |= 0x00;

  gpio_dir(1, 29, 1);
  LPC_IOCON->P1_29 |= 0x00;

  uart3_init();
  i2c2_init();
  i2c_data = 0;

  received = 0;
  sendDataToMP3(0x09, 0x00, 0x02);
  while (received != 1)
    ;

  // stop playback
  received = 0;
  sendDataToMP3(0x16, 0x00, 0x00);
  while (received != 1)
    ;

  printf("=====1======");

  while (1) {

    if (i2c_data == 1) {
      // play background startup music
      received = 0;
      sendDataToMP3(0x08, 0x00, 0x01);
      printf("=====3======");

      i2c_data = 0;
    } else if (i2c_data == 2) {

      // play are you ready
      received = 0;
      sendDataToMP3(0x0f, 0x01, 0x02);
      while (received != 1)
        ;
      printf("=====4======");

      i2c_data = 0;
    } else if (i2c_data == 3) {

      // play bullet
      received = 0;
      // sendDataToMP3(0x0f, 0x01, 0x03);
      sendDataToMP3(0x0f, 0x01, 0x05);
      while (received != 1)
        ;
      printf("=====5======");

      i2c_data = 0;
    } else if (i2c_data == 4) {

      // play collision
      received = 0;
      sendDataToMP3(0x0f, 0x01, 0x04);
      while (received != 1)
        ;
      printf("=====6======");

      i2c_data = 0;
    }
    // } else {

    //   if (i2c_data != 99) {

    //     i2c_data = 99;

    //     received = 0;
    //     sendDataToMP3(0x08, 0x00, 0x01);
    //     printf("=====3======");
    //   }
    // }
  }

  // if (gpio_get(0, 29)) {
  //   delay__ms(20);
  //   while (gpio_get(0, 29))
  //     ;
  //   printf("=====2A======");
  //   state = 1;
  // }

  // if (gpio_get(0, 30)) {
  //   delay__ms(20);
  //   while (gpio_get(0, 30))
  //     ;
  //   printf("=====2B======");
  //   state = 2;
  // }

  // if (gpio_get(2, 2)) {

  //   state = 1;
  // }

  // if (gpio_get(2, 0)) {

  //   state = 2;
  // }

  return 0;
}