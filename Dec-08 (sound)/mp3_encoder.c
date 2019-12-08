#include "mp3_encoder.h"
#include "gpio_driver.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "stdbool.h"
#include "stdint.h"
#include "uart.h"

typedef struct {
  LPC_I2C_TypeDef *const registers; ///< LPC memory mapped registers for the I2C bus

  // These are the parameters we save before a transaction is started
  uint8_t error_code;
  uint8_t slave_address;
  uint8_t starting_slave_memory_address;

  uint8_t *input_byte_pointer;        ///< Used for reading I2C slave device
  const uint8_t *output_byte_pointer; ///< Used for writing data to the I2C slave device
  size_t number_of_bytes_to_transfer;
} i2c_s;

void u3_isr(void);
void i2c_isr(void);
bool i2c__handle_state_machine(i2c_s *i2c);

void i2c_isr(void) { gpio_set(2, 3, 1); }

void u3_isr(void) {

  uint8_t RDR = 1;

  char data;

  if (LPC_UART3->LSR & RDR) {
    data = LPC_UART3->RBR;
  }

  if (data == 0xef) {
    received = 1;
  }

  printf("%x\r\n", data);
}

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
  uint16_t data = 625;
  // const uint16_t data = (uint16_t)((96000000 / ((16 * 38400))));
  LPC_UART3->DLL = (data & 0xFF);
  LPC_UART3->DLM = ((data >> 8) & 0xFF);

  // clears DLAB bit
  LPC_UART3->LCR &= ~(1 << 7);

  // selects word length = 8 bit
  LPC_UART3->LCR |= 0x3;

  // enable interrupt
  LPC_UART3->IER |= 0x1;

  // enable NVIC
  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__UART3, u3_isr);
}

void i2c2_init(void) {

  LPC_I2C2->CONCLR = 0x6C; // Clear ALL I2C Flags

  LPC_I2C2->MASK0 = 0x00;
  LPC_I2C2->MASK1 = 0x00;
  LPC_I2C2->MASK2 = 0x00;
  LPC_I2C2->MASK3 = 0x00;

  LPC_I2C2->ADR0 = 0x46;
  LPC_I2C2->ADR1 = 0x00;
  LPC_I2C2->ADR2 = 0x00;
  LPC_I2C2->ADR3 = 0x00;

  // set to slave
  LPC_I2C2->CONSET = 0x44;
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
    // printf("%c\r\n", *input_byte);
  }

  return true;
}

bool uart3_get(char *input_byte) {

  uint8_t RDR = 1;

  if (LPC_UART3->LSR & RDR) {
    *input_byte = LPC_UART3->RBR;
    // printf("%c\r\n", *input_byte);
    return true;
  } else {
    return false;
  }
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

bool sendDataToMP3(char cmd, uint8_t msb, uint8_t lsb) {

  uart3_put(0x7E);       // start
  uart3_put(0xFF);       // version, always 0xFF
  uart3_put(0x06);       // length of data, always 0x06
  uart3_put(cmd & 0xFF); // command
  uart3_put(0x01);       // feedback, set to 1 to receive ack
  uart3_put(msb);        // dataHi
  uart3_put(lsb);        // dataLo
  // 2 bytes of CRC ignored
  uart3_put(0xEF); // stop

  return true;
}