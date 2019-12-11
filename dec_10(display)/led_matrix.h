#pragma once

#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"

void init_led_matrix();
void led_matrix_control(int type, bool state);
void updateDisplay(void);
int8_t updatePixel(uint8_t x, uint8_t y, uint8_t color);
void drawSpaceship(uint8_t x, uint8_t y, uint8_t color);
void clearDisplay(void);
void drawEnemy(uint8_t x, uint8_t y, uint8_t color);
void firing_position(void);
void enemy1_firing_position(void);
// void burst_animation(uint8_t x, uint8_t y, uint8_t color);
void drawEnemy_new(uint8_t x, uint8_t y, uint8_t color);
void drawVillain(uint8_t x, uint8_t y, uint8_t color);
// void firing_start_position(uint8_t x, uint8_t y, uint8_t color);

// void checkCollision(void); // collision.c api
enum { R1, G1, B1, R2, G2, B2, A, B, C, D, CLK, LAT, OE };

uint8_t matrixbuff[32][64];

struct {

  uint8_t spacecraft[40][2];
  uint8_t enemy[100][20][2]; // no of enemies, no of pixels per enemy, x & y co ordinates
  uint8_t enemy_count;

} position;

bool bullet_collision_1, bullet_collision_2, spacecraft_collision_1, spacecraft_collision_2;

bool b2b_collision_1, b2b_collision_2, b2b_collision_3;

bool b2ship_collision1, b2ship_collision2, b2ship_collision3;

bool villian_inc_flag;

uint8_t index1;
uint8_t index2;
uint8_t bullet_count;
uint8_t bulletarray[100][2];

int x1, x2, x3, x4, y1, y2, enemy_color_1, enemy_color_2;
uint8_t firing_position_x, firing_position_y, enemy1_firing_position_x, enemy1_firing_position_y, ship_position_x,
    ship_position_y, villian_position_x, villian_position_y;
uint64_t data;

uint8_t villian_bullet1_pos_x, villian_bullet1_pos_y, villian_bullet2_pos_x, villian_bullet2_pos_y,
    villian_bullet3_pos_x, villian_bullet3_pos_y;

volatile uint16_t kill_count, health, villian_health;

bool shootbool;

uint8_t level;

#define EXPIRED 64

#endif