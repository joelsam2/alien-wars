#include "FreeRTOS.h"
#include "alien_adc.h"
#include "alien_uart.h"
#include "board_io.h"
#include "common_macros.h"
#include "gpio.h"
#include "gpio_driver.h"
#include "led_matrix.h"
#include "sj2_cli.h"
#include "task.h"
#include <stdio.h>
#include <string.h>

void bluetooth_transmit(void *ptr);
void bluetooth_receive(void *p);

//#define TRANSMITTER
#define RECEIVER
//#define LOOPBACK

int main(void) {

#ifdef TRANSMITTER
  adc__initialize_alien();
  uart__init_alien_uart3(LPC_PERIPHERAL__UART3, 96000000, 38400);
  xTaskCreate(bluetooth_transmit, "bluetooth_transmit", 1024U, NULL, 3, NULL);
  vTaskStartScheduler();
#endif

#ifdef RECEIVER
  uart__init_alien_uart2(LPC_PERIPHERAL__UART3, 96000000, 38400);
  xTaskCreate(bluetooth_receive, "bluetooth_receive", 1024U, NULL, 3, NULL);
  vTaskStartScheduler();
#endif

#ifdef LOOPBACK
  adc__initialize_alien();
  uart__init_alien_uart2(LPC_PERIPHERAL__UART2, 96000000, 38400);
  xTaskCreate(bluetooth_transmit, "bluetooth_transmit", 1024U, NULL, 3, NULL);
  xTaskCreate(bluetooth_receive, "bluetooth_receive", 1024U, NULL, 3, NULL);
  vTaskStartScheduler();
#endif

  return 0;
}

#ifdef TRANSMITTER
void bluetooth_transmit(void *ptr) {
  while (1) {
    if (!bluetooth_send_data()) {
      printf("Tansmit register full...\n");
    }
    vTaskDelay(20);
  }
}
#endif

#ifdef RECEIVER
void bluetooth_receive(void *p) {
  while (1) {
    if (bluetooth_receive_data()) {
      // printf("Success");
    } else {
      fprintf(stderr, "Receive register empty...\n");
    }
    vTaskDelay(20);
  }
}
#endif

#ifdef LOOPBACK

void bluetooth_transmit(void *ptr) {
  while (1) {
    if (!bluetooth_send_data()) {
      printf("Tansmit register full...\n");
    }
    vTaskDelay(20);
  }
}

void bluetooth_receive(void *p) {
  while (1) {
    // TODO: Use uart_lab__polled_get() function and print the received value
    if (bluetooth_receive_data()) {
      printf("Success");
    } else {
      fprintf(stderr, "Receive register empty...\n");
    }
    vTaskDelay(500);
  }
}

#endif
