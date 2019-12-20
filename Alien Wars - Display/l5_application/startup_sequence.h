#include <stdint.h>

void startup_screen(void);
void game_over_screen(void);
void level_up_screen(uint8_t level_number);
void burst_animation(uint8_t x, uint8_t y, uint8_t color);
void winner(void);