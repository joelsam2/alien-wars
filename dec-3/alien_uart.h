#include "lpc_peripherals.h"
#include <stdint.h>

void uart__init_alien_uart3(lpc_peripheral_e uart, uint32_t peripheral_clock, uint32_t baud_rate);
void uart__init_alien_uart3_receiver(lpc_peripheral_e uart, uint32_t peripheral_clock, uint32_t baud_rate);
bool bluetooth_send_data(void);
bool bluetooth_receive_data(void);

typedef enum joystick {
  center = 0,
  down = 1,
  up = 2,
  left = 3,
  right = 4,
  right_up_diagonal = 5,
  right_down_diagonal = 6,
  left_up_diagonal = 7,
  left_down_diagonal = 8,
  bullet = 9,
  missile = 10

} joystick;