#include "led_matrix.h"

void checkCollision(void) {

  // handle bullet movement and collision with enemy
  for (int i = 0; i < bullet_count; i++) {

    // if y axis is less than 64
    if (bulletarray[i][1] != EXPIRED) {
      // if collision detected then turn off the bullet, else increment the bullet. Also turn off if greater than 64.
      if ((matrixbuff[bulletarray[i][0]][bulletarray[i][1] + 1]) != 0) {
        bulletarray[i][1] = EXPIRED;
      } else {
        bulletarray[i][1]++;
        if (bulletarray[i][1] > 64)
          bulletarray[i][1] = EXPIRED;
      }
    }
  }

  // check if bullet hit enemy spaceship 2
  if ((matrixbuff[x2][y2 - 1] == 7) || (matrixbuff[x2 + 1][y2 - 1] == 7) || (matrixbuff[x2 + 2][y2 - 1] == 7) ||
      (matrixbuff[x2 + 3][y2 - 1] == 7) || (matrixbuff[x2 + 4][y2 - 1] == 7) || (matrixbuff[x2 + 5][y2 - 1] == 7) ||
      (matrixbuff[x2 + 6][y2 - 1] == 7)) {
    bullet_collision_2 = true;
    drawEnemy_new(x2, y2, 0);
    burst_animation(x2, y2, enemy_color_2);
    // updateDisplay();
    // updateDisplay();
    // updateDisplay();
    // updateDisplay();
    // updateDisplay();
    // delay__ms(4);
    // burst_animation(x1, y1, 7);
    // delay__ms(4);
    // burst_animation(x1, y1, 7);
    // delay__ms(4);
    // burst_animation(x1, y1, 7);
    // delay__ms(4);
  }

  // bullet collision for enemy spaceship 1
  if ((matrixbuff[x1][y1 - 1] == 7) || (matrixbuff[x1 + 1][y1 - 1] == 7) || (matrixbuff[x1 + 2][y1 - 1] == 7) ||
      (matrixbuff[x1 + 3][y1 - 1] == 7) || (matrixbuff[x1 + 4][y1 - 1] == 7) || (matrixbuff[x1 + 5][y1 - 1] == 7) ||
      (matrixbuff[x1 + 6][y1 - 1] == 7)) {
    bullet_collision_1 = true;
    drawEnemy_new(x1, y1, 0);
    burst_animation(x1, y1, enemy_color_1);
    // updateDisplay();
    // updateDisplay();
    // updateDisplay();
    // updateDisplay();
    // updateDisplay();
    // delay__ms(4);
    // burst_animation(x1, y1, 7);
    // delay__ms(4);
    // burst_animation(x1, y1, 7);
    // delay__ms(4);
    // burst_animation(x1, y1, 7);
    // delay__ms(4);
  }

  // spacecraft collision for enemy spaceship 2
  if ((matrixbuff[x2][y2 + 2] == 4) || (matrixbuff[x2 + 1][y2 + 1] == 4) || (matrixbuff[x2 + 2][y2] == 4) ||
      (matrixbuff[x2 + 3][y2 - 1] == 4) || (matrixbuff[x2 + 4][y2] == 4) || (matrixbuff[x2 + 5][y2 + 1] == 4) ||
      (matrixbuff[x2 + 6][y2 + 2] == 4) || (matrixbuff[x2 + 1][y2 + 4] == 4) || (matrixbuff[x2 + 1][y2 + 5] == 4) ||
      (matrixbuff[x2 + 3][y2 + 4] == 4) || (matrixbuff[x2 + 3][y2 + 5] == 4) || (matrixbuff[x2 + 5][y2 + 4] == 4) ||
      (matrixbuff[x2 + 5][y2 + 5] == 4)) {
    spacecraft_collision_2 = true;
    drawEnemy_new(x2, y2, 0);
    burst_animation(x2, y2, enemy_color_2);
    // updateDisplay();
    // updateDisplay();
    // updateDisplay();
  } else if ((matrixbuff[x2][y2 + 2] == 2) || (matrixbuff[x2 + 1][y2 + 1] == 2) || (matrixbuff[x2 + 2][y2] == 2) ||
             (matrixbuff[x2 + 3][y2 - 1] == 2) || (matrixbuff[x2 + 4][y2] == 2) || (matrixbuff[x2 + 5][y2 + 1] == 2) ||
             (matrixbuff[x2 + 6][y2 + 2] == 2) || (matrixbuff[x2 + 1][y2 + 4] == 2) ||
             (matrixbuff[x2 + 1][y2 + 5] == 2) || (matrixbuff[x2 + 3][y2 + 4] == 2) ||
             (matrixbuff[x2 + 3][y2 + 5] == 2) || (matrixbuff[x2 + 5][y2 + 4] == 2) ||
             (matrixbuff[x2 + 5][y2 + 5] == 2)) {
    spacecraft_collision_2 = true;
    drawEnemy_new(x2, y2, 0);
    burst_animation(x2, y2, enemy_color_2);
    // updateDisplay();
    // updateDisplay();
    // updateDisplay();
  }

  // spacecraft collision for enemy spaceship 1
  if ((matrixbuff[x1][y1 + 2] == 4) || (matrixbuff[x1 + 1][y1 + 1] == 4) || (matrixbuff[x1 + 2][y1] == 4) ||
      (matrixbuff[x1 + 3][y1 - 1] == 4) || (matrixbuff[x1 + 4][y1] == 4) || (matrixbuff[x1 + 5][y1 + 1] == 4) ||
      (matrixbuff[x1 + 6][y1 + 2] == 4) || (matrixbuff[x1 + 1][y1 + 4] == 4) || (matrixbuff[x1 + 1][y1 + 5] == 4) ||
      (matrixbuff[x1 + 3][y1 + 4] == 4) || (matrixbuff[x1 + 3][y1 + 5] == 4) || (matrixbuff[x1 + 5][y1 + 4] == 4) ||
      (matrixbuff[x1 + 5][y1 + 5] == 4)) {
    spacecraft_collision_1 = true;
    drawEnemy_new(x1, y1, 0);
    burst_animation(x1, y1, enemy_color_1);
    // updateDisplay();
    // updateDisplay();
    // updateDisplay();
  } else if ((matrixbuff[x1][y1 + 2] == 2) || (matrixbuff[x1 + 1][y1 + 1] == 2) || (matrixbuff[x1 + 2][y1] == 2) ||
             (matrixbuff[x1 + 3][y1 - 1] == 2) || (matrixbuff[x1 + 4][y1] == 2) || (matrixbuff[x1 + 5][y1 + 1] == 2) ||
             (matrixbuff[x1 + 6][y1 + 2] == 2) || (matrixbuff[x1 + 1][y1 + 4] == 2) ||
             (matrixbuff[x1 + 1][y1 + 5] == 2) || (matrixbuff[x1 + 3][y1 + 4] == 2) ||
             (matrixbuff[x1 + 3][y1 + 5] == 2) || (matrixbuff[x1 + 5][y1 + 4] == 2) ||
             (matrixbuff[x1 + 5][y1 + 5] == 2)) {
    spacecraft_collision_1 = true;
    drawEnemy_new(x1, y1, 0);
    burst_animation(x1, y1, enemy_color_1);
    // updateDisplay();
    // updateDisplay();
    // updateDisplay();
  }

  // missile collision
}