#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "gpio.h"
#include "sj2_cli.h"

#include "gpio_driver.h"
#include "led_matrix.h"

#include "portmacro.h"
#include "semphr.h"
#include <string.h>

SemaphoreHandle_t display;

void refreshDisplayTask() {

  while (1) {
    xSemaphoreTake(display, portMAX_DELAY);
    updateDisplay();
    xSemaphoreGive(display);
    vTaskDelay(4);
  }
}

void updateDisplayTask() {

  int x, y;

  while (1) {
    xSemaphoreTake(display, portMAX_DELAY);

    for (y = 0; y < 64; y++) {
      clearDisplay();
      drawSpaceship(0, y, 7);
      // delay__ms(10);
    }

    xSemaphoreGive(display);

    // delay__ms(1000);
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

  display = xSemaphoreCreateMutex();
  init_led_matrix();

  LPC_IOCON->P2_3 = 0;
  gpio_dir(2, 3, 0);

  LPC_IOCON->P0_29 = 0;
  gpio_dir(0, 29, 1);

  puts("Starting RTOS");

  while (1) {

    for (int y = 0, x = 0; x < 32; x++) {
      clearDisplay();
      drawSpaceship(x, 0, 7);
      updateDisplay();
      delay__ms(10);
    }
  }

  // xTaskCreate(clearDisplayTask, "clearDisplayTask", 1024, NULL, 1, NULL);
  xTaskCreate(updateDisplayTask, "updateDisplayTask", 2048, NULL, 1, NULL);
  xTaskCreate(refreshDisplayTask, "refreshDisplayTask", 2048, NULL, 2, NULL);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}
