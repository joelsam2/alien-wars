#include "mp3_encoder.h"
#include "gpio_driver.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "stdbool.h"
#include "stdint.h"
#include "uart.h"

void uart3_init(void) {

  gpio_dir(4, 28, 0);
  gpio_dir(4, 29, 0);

  LPC_IOCON->P4_28 |= 0x02;
  LPC_IOCON->P4_29 |= 0x02;

  LPC_SC->PCLKSEL = 1;

  lpc_peripheral__turn_on_power_to(LPC_PERIPHERAL__UART3);

  // set DLAB bit to access DLL and DLM
  LPC_UART3->LCR |= 1 << 7;

  // baud rate = 9600
  // uint16_t data = 625;
  const uint16_t data = (uint16_t)((96000000 / ((16 * 38400))));
  LPC_UART3->DLL = (data & 0xFF);
  LPC_UART3->DLM = ((data >> 8) & 0xFF);

  // clears DLAB bit
  LPC_UART3->LCR &= ~(1 << 7);

  // selects word length = 8 bit
  LPC_UART3->LCR |= 0x3;
}

void uart2_init(void) {

  // gpio_dir(2, 8, 0);
  // gpio_dir(2, 9, 0);

  // LPC_IOCON->P2_8 |= 0x02;
  // LPC_IOCON->P2_9 |= 0x02;

  LPC_SC->PCLKSEL = 1;

  // lpc_peripheral__turn_on_power_to(LPC_PERIPHERAL__UART2);
  LPC_SC->PCONP |= (1 << 8);

  // set DLAB bit to access DLL and DLM
  LPC_UART4->LCR |= 1 << 7;

  // baud rate = 9600
  uint16_t data = 625;
  // const uint16_t data = (uint16_t)((96000000 / ((16 * 38400))));
  LPC_UART4->DLL = (data & 0xFF);
  LPC_UART4->DLM = ((data >> 8) & 0xFF);

  // clears DLAB bit
  LPC_UART4->LCR &= ~(1 << 7);

  // selects word length = 8 bit
  LPC_UART4->LCR |= 0x3;
}

bool uart2_get(char *input_byte) {

  uint8_t RDR = 1;

  if (LPC_UART4->LSR & RDR) {
    *input_byte = LPC_UART4->RBR;
    printf("%c\r\n", *input_byte);
  }

  return true;
}

bool uart3_get(char *input_byte) {

  uint8_t RDR = 1;

  if (LPC_UART3->LSR & RDR) {
    *input_byte = LPC_UART3->RBR;
    printf("%c\r\n", *input_byte);
  }

  return true;
}

bool uart2_put(char output_byte) {

  uint8_t THRE = 5;

  if (LPC_UART4->LSR & (1 << THRE)) {
    LPC_UART4->THR = output_byte;
    while (!(LPC_UART4->LSR & (1 << THRE)))
      ;
  }

  return true;
}

bool uart3_put(char output_byte) {

  uint8_t THRE = 5;

  if (LPC_UART3->LSR & (1 << THRE)) {
    LPC_UART3->THR = output_byte;
    while (!(LPC_UART3->LSR & (1 << THRE)))
      ;
  }

  return true;
}

void sendDataToMP3(int cmd, int data) {

  uart2_put(0x7E);               // start
  uart2_put(0xFF);               // version, always 0xFF
  uart2_put(0x06);               // length of data, always 0x06
  uart2_put(cmd & 0xFF);         // command
  uart2_put(0x00);               // feedback, set to 1 to receive ack
  uart2_put((data >> 8) & 0xFF); // dataHi
  uart2_put(data & 0xFF);        // dataLo
  // 2 bytes of CRC ignored
  uart2_put(0xEF); // stop
}

void play(uint8_t track) {

  if (track == BACKGROUND) {
    i2c__write_single(2, 0x46, 0x09, 0x01);
  } else if (track == AREYOUREADY) {
    i2c__write_single(2, 0x46, 0x09, 0x02);
  } else if (track == BULLET) {
    i2c__write_single(2, 0x46, 0x09, 0x03);
  } else if (track == COLLISION) {
    i2c__write_single(2, 0x46, 0x09, 0x04);
  }
}