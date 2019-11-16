#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "gpio.h"
#include "sj2_cli.h"

#include "gpio_driver.h"
#include "led_matrix.h"

#include <string.h>

void refreshDisplayTask() {

  while (1) {
    updateDisplay();
    vTaskDelay(4);
  }
}

void updateDisplayTask() {

  while (1) {
    updatePixel(0, 0, 1);
    // vTaskDelay(1);
  }
}

void clearDisplayTask() {

  while (1) {
    memset(matrixbuff, 0, 32 * 64);
  }
}

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

  init_led_matrix();

  LPC_IOCON->P2_3 = 0;
  gpio_dir(2, 3, 0);

  LPC_IOCON->P0_29 = 0;
  gpio_dir(0, 29, 1);

  puts("Starting RTOS");

  // xTaskCreate(clearDisplayTask, "clearDisplayTask", 1024, NULL, 1, NULL);
  xTaskCreate(updateDisplayTask, "updateDisplayTask", 1024, NULL, 2, NULL);
  xTaskCreate(refreshDisplayTask, "refreshDisplayTask", 1024, NULL, 3, NULL);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}
