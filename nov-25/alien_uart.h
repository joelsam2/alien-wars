#include "lpc_peripherals.h"
#include <stdint.h>

void uart__init_alien_uart3(lpc_peripheral_e uart, uint32_t peripheral_clock, uint32_t baud_rate);
void uart__init_alien_uart2(lpc_peripheral_e uart, uint32_t peripheral_clock, uint32_t baud_rate);
bool bluetooth_send_data(void);
bool bluetooth_receive_data(void);

typedef enum joystick {
  center = 0,
  down,
  up,
  left,
  right

} joystick;