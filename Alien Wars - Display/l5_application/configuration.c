#include "led_matrix.h"

void configuration_init(void) {

  ship_position_x = 15;
  ship_position_y = 0;
  villian_position_x = 15;
  villian_position_y = 45;
  position.enemy_count = 0;
  index1 = 0;
  index2 = 0;
  bullet_count = 0;
  shootbool = false;

  level = 1;

  kill_count = 0;
  health = 100;

  villian_health = 50;

  bullet_collision_1 = false;
  spacecraft_collision_1 = false;
  bullet_collision_2 = false;
  spacecraft_collision_2 = false;

  b2b_collision_1 = false;
  b2b_collision_2 = false;
  b2b_collision_3 = false;

  b2ship_collision1 = false;
  b2ship_collision2 = false;
  b2ship_collision3 = false;

  villian_inc_flag = true;

  x1 = 100;
  y1 = 100;
  x2 = 100;
  y2 = 100;
}