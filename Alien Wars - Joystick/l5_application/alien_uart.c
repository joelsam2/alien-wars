#include "alien_uart.h"
#include "FreeRTOS.h"
#include "alien_adc.h"
#include "gpio.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "queue.h"
#include <stdio.h>

extern bool joystick_start_flag;

#define TRANSMITTER
//#define RECEIVER

//#define LOOPBACK

uint8_t data;
uint8_t received_data;
joystick joystick_control_signal;

extern bool bullet_flag;
extern bool missile_flag;

bool down_flag;
bool up_flag;
bool right_flag;
bool left_flag;
bool right_up_diagonal_flag;
bool right_down_diagonal_flag;
bool left_up_diagonal_flag;
bool left_down_diagonal_flag;

#ifdef TRANSMITTER
uint8_t send_data_alien(void) {

  uint16_t value_x = get_value_x();
  printf("X: %d\n", value_x);
  uint16_t value_y = get_value_y();
  printf("Y: %d\n", value_y);

  if (bullet_flag == true) {
    data = 9;
    bullet_flag = false;
  } else if (missile_flag == true) {
    data = 10;
    missile_flag = false;
  } else if ((value_x > 150 && value_x < 230) && (value_y > 150 && value_y < 230))
    data = 0; // center
  else if ((value_x < 150) && (value_y > 150 && value_y < 230))
    data = 3; // left
  else if ((value_x > 240) && (value_y > 150 && value_y < 230))
    data = 4; // right
  else if ((value_x > 150 && value_x < 230) && (value_y < 150))
    data = 1; // down
  else if ((value_x > 150 && value_x < 230) && (value_y > 240))
    data = 2; // up
  else if ((value_x > 320) && (value_y > 320))
    data = 5; // right up diagonal
  else if ((value_x > 310) && (value_y < 50))
    data = 6; // right down diagonal
  else if ((value_x < 100) && (value_y > 290))
    data = 7; // left up diagonal
  else if ((value_x < 50) && (value_y < 50))
    data = 8; // left down diagonal

  printf("data: %d\n", data);

  return data;
}
#endif

#ifdef TRANSMITTER
// UART3 -> Tx
void uart__init_alien_uart3(lpc_peripheral_e uart, uint32_t peripheral_clock, uint32_t baud_rate) {
  lpc_peripheral__turn_on_power_to(uart); // LPC_PERIPHERAL__UART3
  // LPC_SC->PCONP |= (1 << 25);

  // Set GPIO pin function
  LPC_IOCON->P0_0 |= (1 << 1); // Tx
  LPC_IOCON->P0_1 |= (1 << 1); // Rx

  LPC_GPIO0->DIR &= ~(1 << 29); // bullet
  LPC_GPIO0->DIR &= ~(1 << 30); // missile

  LPC_GPIO0->SET = (1 << 7); // Provide Power = 3.3v

  uint8_t word_length = 3; // 8-bit UART
  uint8_t dlab_bit = (uint8_t)(1 << 7);
  LPC_UART3->LCR = (dlab_bit | word_length); // Set DLAB bit to access DLM & DLL

  const uint16_t divider = (uint16_t)((peripheral_clock / ((16 * baud_rate))));
  LPC_UART3->DLM = (divider >> 8) & 0xFF;
  LPC_UART3->DLL = (divider >> 0) & 0xFF;

  LPC_UART3->LCR &= ~dlab_bit;
}
#endif

#ifdef RECEIVER
// UART2 -> Rx
void uart__init_alien_uart3_receiver(lpc_peripheral_e uart, uint32_t peripheral_clock, uint32_t baud_rate) {
  lpc_peripheral__turn_on_power_to(uart); // LPC_PERIPHERAL__UART2
  LPC_SC->PCONP |= (1 << 25);

  // Set GPIO pin function
  LPC_IOCON->P4_28 |= (1 << 1); // Tx
  LPC_IOCON->P4_29 |= (1 << 1); // Rx

  LPC_GPIO0->SET = (1 << 7); // Provide Power = 3.3v

  uint8_t word_length = 3; // 8-bit UART
  uint8_t dlab_bit = (uint8_t)(1 << 7);
  LPC_UART3->LCR = (dlab_bit | word_length); // Set DLAB bit to access DLM & DLL

  const uint16_t divider = (uint16_t)((peripheral_clock / ((16 * baud_rate))));
  LPC_UART3->DLM = (divider >> 8) & 0xFF;
  LPC_UART3->DLL = (divider >> 0) & 0xFF;

  LPC_UART3->LCR &= ~dlab_bit;
}
#endif

#ifdef TRANSMITTER
bool bluetooth_send_data(void) {
  volatile uint8_t data;

  if ((LPC_UART3->LSR >> 5) & 0x1) {

    data = send_data_alien();
    LPC_UART3->THR = data;
    return true;
  } else {
    return false;
  }
}
#endif

#ifdef RECEIVER
void set_joystick_control_signal(void) {

  if (received_data == 1)
    joystick_control_signal = down;
  else if (received_data == 2)
    joystick_control_signal = up;
  else if (received_data == 3)
    joystick_control_signal = left;
  else if (received_data == 4)
    joystick_control_signal = right;
  else if (received_data == 5)
    joystick_control_signal = right_up_diagonal;
  else if (received_data == 6)
    joystick_control_signal = right_down_diagonal;
  else if (received_data == 7)
    joystick_control_signal = left_up_diagonal;
  else if (received_data == 8)
    joystick_control_signal = left_down_diagonal;
  else if (received_data == 9)
    joystick_control_signal = bullet;
  else
    joystick_control_signal = center;
}
#endif

#ifdef RECEIVER
bool bluetooth_receive_data(void) {
  if ((LPC_UART3->LSR) & 1) {
    received_data = LPC_UART3->RBR;
    set_joystick_control_signal();
    printf("Value in RBR %d\n", received_data);
    return true;
  } else {
    return false;
  }
}
#endif

#ifdef LOOPBACK

void uart__init_alien_uart2(lpc_peripheral_e uart, uint32_t peripheral_clock, uint32_t baud_rate) {
  lpc_peripheral__turn_on_power_to(uart); // LPC_PERIPHERAL__UART2
  LPC_SC->PCONP |= (1 << 24);

  // Set GPIO pin function
  LPC_IOCON->P0_10 |= (1 << 0); // Tx
  LPC_IOCON->P0_11 |= (1 << 0); // Rx

  LPC_GPIO0->SET = (1 << 7); // Provide Power = 3.3v

  uint8_t word_length = 3; // 8-bit UART
  uint8_t dlab_bit = (uint8_t)(1 << 7);
  LPC_UART2->LCR = (dlab_bit | word_length); // Set DLAB bit to access DLM & DLL

  const uint16_t divider = (uint16_t)((peripheral_clock / ((16 * baud_rate))));
  LPC_UART2->DLM = (divider >> 8) & 0xFF;
  LPC_UART2->DLL = (divider >> 0) & 0xFF;

  LPC_UART2->LCR &= ~dlab_bit;
}

uint8_t send_data_alien(void) {

  uint16_t value_x = get_value_x();
  uint16_t value_y = get_value_y();

  if ((value_x > 150 && value_x < 230) && (value_y > 150 && value_y < 230))
    data = 0; // center
  else if ((value_x < 150) && (value_y > 150 && value_y < 230))
    data = 1; // down
  else if ((value_x > 240) && (value_y > 150 && value_y < 230))
    data = 2; // up
  else if ((value_x > 150 && value_x < 230) && (value_y < 150))
    data = 3; // left
  else if ((value_x > 150 && value_x < 230) && (value_y > 240))
    data = 4; // right

  printf("data: %d\n", data);

  return data;
}

bool bluetooth_send_data(void) {
  volatile uint8_t data;

  if ((LPC_UART2->LSR >> 5) & 0x1) {

    data = send_data_alien();
    LPC_UART2->THR = data;
    return true;
  } else {
    return false;
  }
}

void set_joystick_control_signal(void) {

  if (received_data == 1)
    joystick_control_signal = down;
  else if (received_data == 2)
    joystick_control_signal = up;
  else if (received_data == 3)
    joystick_control_signal = left;
  else if (received_data == 4)
    joystick_control_signal = right;
  else
    joystick_control_signal = center;
}

bool bluetooth_receive_data(void) {
  if ((LPC_UART2->LSR) & 0x1) {
    received_data = LPC_UART2->RBR;
    set_joystick_control_signal();
    printf("Value in RBR %d\n", received_data);
    return true;
  } else {
    return false;
  }
}

#endif
