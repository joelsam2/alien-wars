#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "gpio.h"
#include "sj2_cli.h"

#include "gpio_driver.h"
#include "led_matrix.h"

int main(void) {

  // xTaskCreate(blink_task, "led0", configMINIMAL_STACK_SIZE, (void *)&led0, PRIORITY_LOW, NULL);
  // xTaskCreate(blink_task, "led1", configMINIMAL_STACK_SIZE, (void *)&led1, PRIORITY_LOW, NULL);

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

  int row = 0;

  // led_matrix_control(OE, true);
  // led_matrix_control(LAT, false);

  // led_matrix_control(A, row & (1 << 1));
  // led_matrix_control(B, row & (1 << 2));
  // led_matrix_control(C, row & (1 << 3));
  // led_matrix_control(D, row & (1 << 4));

  // led_matrix_control(R1, false);
  // led_matrix_control(G1, false);
  // led_matrix_control(B1, false);

  // led_matrix_control(R2, false);
  // led_matrix_control(G2, false);
  // led_matrix_control(B2, false);

  // for(int i =0; i<64; i++){
  //   led_matrix_control(CLK, true);
  //   led_matrix_control(CLK, false);
  // }

  // led_matrix_control(LAT, true);
  // led_matrix_control(OE, false);

  while (1) {
    // if (gpio_get(0, 29) == true) {

    //   while (gpio_get(0, 29) != false)
    //     ;

    led_matrix_control(OE, true);

    led_matrix_control(A, row & (1));
    led_matrix_control(B, row & (1 << 1));
    led_matrix_control(C, row & (1 << 2));
    led_matrix_control(D, row & (1 << 3));

    led_matrix_control(R1, false);
    led_matrix_control(G1, false);
    led_matrix_control(B1, false);

    led_matrix_control(R2, true);
    led_matrix_control(G2, false);
    led_matrix_control(B2, false);

    led_matrix_control(CLK, true);
    led_matrix_control(CLK, false);

    led_matrix_control(LAT, true);
    led_matrix_control(LAT, false);
    led_matrix_control(OE, false);

    row++;
    delay__ms(1000);
    // }
  }

  puts("Starting RTOS");
  // vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}

static void blink_task(void *params) {
  const gpio_s led = *((gpio_s *)params);

  // Warning: This task starts with very minimal stack, so do not use printf() API here to avoid stack overflow
  while (true) {
    gpio__toggle(led);
    vTaskDelay(500);
  }
}

// This sends periodic messages over printf() which uses system_calls.c to send them to UART0
static void uart_task(void *params) {
  TickType_t previous_tick = 0;
  TickType_t ticks = 0;

  while (true) {
    // This loop will repeat at precise task delay, even if the logic below takes variable amount of ticks
    vTaskDelayUntil(&previous_tick, 2000);

    /* Calls to fprintf(stderr, ...) uses polled UART driver, so this entire output will be fully
     * sent out before this function returns. See system_calls.c for actual implementation.
     *
     * Use this style print for:
     *  - Interrupts because you cannot use printf() inside an ISR
     *  - During debugging in case system crashes before all output of printf() is sent
     */
    ticks = xTaskGetTickCount();
    fprintf(stderr, "%u: This is a polled version of printf used for debugging ... finished in", (unsigned)ticks);
    fprintf(stderr, " %lu ticks\n", (xTaskGetTickCount() - ticks));

    /* This deposits data to an outgoing queue and doesn't block the CPU
     * Data will be sent later, but this function would return earlier
     */
    ticks = xTaskGetTickCount();
    printf("This is a more efficient printf ... finished in");
    printf(" %lu ticks\n\n", (xTaskGetTickCount() - ticks));
  }
}
