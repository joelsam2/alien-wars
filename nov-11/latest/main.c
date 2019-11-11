#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "gpio.h"
#include "sj2_cli.h"

#include "gpio_driver.h"
#include "led_matrix.h"
void rows(uint8_t row);
void task(void *ptr);
void top();
void bottom();

uint64_t ship_array[32] = {0B0000000000000000000000000000000000000000000000000000000000000000,
                           0B0000000000000000000000000000000000000000000000000000000000000000,
                           0B0000000000000111111001000000010011111001000001000000000000000000,
                           0B0000000000000100001001000000010010000001100001000000000000000000,
                           0B0000000000000100001001000000010010000001010001000000000000000000,
                           0B0000000000000111111001000000010011111001001001000000000000000000,
                           0B0000000000000100001001000000010010000001000101000000000000000000,
                           0B0000000000000100001001000000010010000001000011000000000000000000,
                           0B0000000000000100001001111110010011111001000001000000000000000000,
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
                           0B0000000000000000000000000000000000000000000000000000000000000000,
                           0B0000000000000100000100111111001111110011111100000000000000000000,
                           0B0000000000000100000100100001001000100010000000000000000000000000,
                           0B0000000000000100000100100001001001000010000000000000000000000000,
                           0B0000000000000100100100111111001010000011111100000000000000000000,
                           0B0000000000000100100100100001001101000000000100000000000000000000,
                           0B0000000000000100100100100001001000100000000100000000000000000000,
                           0B0000000000000111111100100001001000010011111100000000000000000000,
                           0B00000000000000000000000000000000000000000000000000000000000000000,
                           0B0000000000000000000000000000000000000000000000000000000000000000,
                           0B0000000000000000000000000000000000000000000000000000000000000000};

int main(void) {

#if 0
  // printf() takes more stack space, size this tasks' stack higher
  xTaskCreate(uart_task, "uart", (512U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
#else
  sj2_cli__init();
#endif

  init_led_matrix();

  LPC_IOCON->P2_3 = 0;
  gpio_dir(2, 3, 0);

  LPC_IOCON->P0_29 = 0;
  gpio_dir(0, 29, 1);

  puts("Starting RTOS");
  xTaskCreate(task, "led0", 1024U, NULL, PRIORITY_HIGH, NULL);
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}

void task(void *ptr) {
  while (1) {

    top();
    bottom();
    vTaskDelay(1);
  }
}

void top() {
  for (int i = 0; i < 16; i++) {

    LPC_GPIO2->SET = (1 << 6); // OE
    rows(i);
    LPC_GPIO2->PIN &= ~(1 << 8); // latchs

    for (int j = 64; j > 0; j--) {

      uint64_t R0 = (ship_array[i]) >> j;
      uint64_t G0 = (ship_array[i]) >> j;
      uint64_t B0 = (ship_array[i]) >> j;

      if (R0 & 1)
        LPC_GPIO0->PIN |= (1 << 10);
      else
        LPC_GPIO0->PIN &= ~(1 << 10);

      if (G0 & 1)
        LPC_GPIO0->PIN |= (1 << 1);
      else
        LPC_GPIO0->PIN &= ~(1 << 1);

      if (B0 & 1)
        LPC_GPIO0->PIN |= (1 << 18);
      else
        LPC_GPIO0->PIN &= ~(1 << 18);

      LPC_GPIO0->PIN |= (1 << 16);  // clk
      LPC_GPIO0->PIN &= ~(1 << 16); // clk
    }

    LPC_GPIO2->PIN |= (1 << 8); // latch
    LPC_GPIO2->CLR = (1 << 6);  // OE
  }
}
void bottom() {

  for (int i = 16; i < 32; i++) {

    LPC_GPIO2->SET = (1 << 6); // OE
    rows(i);
    LPC_GPIO2->PIN &= ~(1 << 8); // latchs

    for (int j = 64; j > 0; j--) {

      uint64_t R1 = (ship_array[i]) >> j;
      uint64_t G1 = (ship_array[i]) >> j;
      uint64_t B1 = (ship_array[i]) >> j;

      if (R1 & 1)
        LPC_GPIO0->PIN |= (1 << 15);
      else
        LPC_GPIO0->PIN &= ~(1 << 15);

      if (G1 & 1)
        LPC_GPIO2->PIN |= (1 << 9);
      else
        LPC_GPIO2->PIN &= ~(1 << 9);

      if (B1 & 1)
        LPC_GPIO2->PIN |= (1 << 7);
      else
        LPC_GPIO2->PIN &= ~(1 << 7);

      LPC_GPIO0->PIN |= (1 << 16);  // clk
      LPC_GPIO0->PIN &= ~(1 << 16); // clk
    }

    LPC_GPIO2->PIN |= (1 << 8); // latch
    LPC_GPIO2->CLR = (1 << 6);  // OE
  }
}

void rows(uint8_t row) {
  volatile uint8_t d = (row >> 0) & 1;
  volatile uint8_t c = (row >> 1) & 1;
  volatile uint8_t b = (row >> 2) & 1;
  volatile uint8_t a = (row >> 3) & 1;

  a ? (LPC_GPIO0->PIN |= (1 << 17)) : ((LPC_GPIO0->PIN &= ~(1 << 17)));
  b ? (LPC_GPIO0->PIN |= (1 << 22)) : (LPC_GPIO0->PIN &= ~(1 << 22));
  c ? (LPC_GPIO0->PIN |= (1 << 0)) : (LPC_GPIO0->PIN &= ~(1 << 0));
  d ? (LPC_GPIO0->PIN |= (1 << 11)) : (LPC_GPIO0->PIN &= ~(1 << 11));
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