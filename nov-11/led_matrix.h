
#include "stdbool.h"

void init_led_matrix();
void led_matrix_control(int type, bool state);

enum { R1, G1, B1, R2, G2, B2, A, B, C, D, CLK, LAT, OE };
