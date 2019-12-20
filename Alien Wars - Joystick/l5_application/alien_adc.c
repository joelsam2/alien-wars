#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include <alien_adc.h>

void adc__initialize_alien(void) {

  LPC_SC->PCONP |= (1 << 12); // Turn on  ADC
  LPC_GPIO0->SET = (1 << 6);  // Provide Power to joystick

  // Make ADC operational
  LPC_ADC->CR |= (1 << 21);

  // Setting ADC clock = 12Mhz
  const uint8_t clk_divider = 8;
  LPC_ADC->CR |= (clk_divider << 8);

  // Select two ADC processors
  LPC_ADC->CR |= (1 << 2);
  LPC_ADC->CR |= (1 << 3);

  // selecting pin function
  LPC_IOCON->P0_25 = 0x00000001; // ADC2 -> x-axis -> P0_25
  LPC_IOCON->P0_26 = 0x00000001; // ADC3 -> y-axis -> P0_26

  // Start burst mode
  LPC_ADC->CR |= (1 << 16);
}

// ADC2 -> x-axis -> P0_25
uint16_t get_value_x(void) {
  const uint16_t mask = 0x0FFF;
  uint16_t result = (LPC_ADC->DR[2] >> 4) & mask;
  return result;
}

// ADC3 -> y-axis -> P0_26
uint16_t get_value_y(void) {
  const uint16_t mask = 0x0FFF;
  uint16_t result = (LPC_ADC->DR[3] >> 4) & mask;
  return result;
}
