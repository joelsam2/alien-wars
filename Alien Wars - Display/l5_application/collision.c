#include "led_matrix.h"
#include "startup_sequence.h"

void checkCollision(void) {

  // handle bullet movement and collision with enemy
  for (int i = 0; i < bullet_count; i++) {

    // index 0 is x axis, 1 is y axis
    // if y axis is less than 64
    if (bulletarray[i][1] < 64) {
      // if collision detected then turn off the bullet, else increment the bullet. Also turn off if greater than 63.
      if ((matrixbuff[bulletarray[i][0]][bulletarray[i][1] + 1]) != 0) {
        bulletarray[i][1] = EXPIRED;
      } else {
        bulletarray[i][1]++;
        if (bulletarray[i][1] > 62)
          bulletarray[i][1] = EXPIRED;
      }
    }
  }

  // check all the following points on the aliens if there is a bullet
  // check if bullet hit enemy spaceship 2
  if ((matrixbuff[x2][y2 - 1] == 7) || (matrixbuff[x2 + 1][y2 - 1] == 7) || (matrixbuff[x2 + 2][y2 - 1] == 7) ||
      (matrixbuff[x2 + 3][y2 - 1] == 7) || (matrixbuff[x2 + 4][y2 - 1] == 7) || (matrixbuff[x2 + 5][y2 - 1] == 7) ||
      (matrixbuff[x2 + 6][y2 - 1] == 7)) {
    puts("\nEnemy 2 bullet collision");
    bullet_collision_2 = true;
    kill_count++;
    drawEnemy_new(x2, y2, 0);
    burst_animation(x2, y2, enemy_color_2);
    x2 = EXPIRED;
    y2 = EXPIRED;
  }

  // bullet collision for enemy spaceship 1
  if ((matrixbuff[x1][y1 - 1] == 7) || (matrixbuff[x1 + 1][y1 - 1] == 7) || (matrixbuff[x1 + 2][y1 - 1] == 7) ||
      (matrixbuff[x1 + 3][y1 - 1] == 7) || (matrixbuff[x1 + 4][y1 - 1] == 7) || (matrixbuff[x1 + 5][y1 - 1] == 7) ||
      (matrixbuff[x1 + 6][y1 - 1] == 7)) {
    puts("\nEnemy 1 bullet collision");
    bullet_collision_1 = true;
    kill_count++;
    drawEnemy_new(x1, y1, 0);
    burst_animation(x1, y1, enemy_color_1);
    x1 = EXPIRED;
    y1 = EXPIRED;
  }

  // check spacecraft collision with aliens, we are checking collision with respect to alien co-ordinates. Hence two
  // cases are involved becuase of the color change of spacecraft when health changes
  if (health >= 50) {

    // when spacecraft is green in color
    if (spacecraft_collision_2 != true) {
      if ((matrixbuff[x2][y2 + 2] == 2) || (matrixbuff[x2 + 1][y2 + 1] == 2) || (matrixbuff[x2 + 2][y2] == 2) ||
          (matrixbuff[x2 + 3][y2 - 1] == 2) || (matrixbuff[x2 + 4][y2] == 2) || (matrixbuff[x2 + 5][y2 + 1] == 2) ||
          (matrixbuff[x2 + 6][y2 + 2] == 2) || (matrixbuff[x2][y2 + 4] == 2) || (matrixbuff[x2][y2 + 5] == 2) ||
          (matrixbuff[x2][y2 + 6] == 2) || (matrixbuff[x2 + 1][y2 + 4] == 2) || (matrixbuff[x2 + 2][y2 + 5] == 2) ||
          (matrixbuff[x2 + 2][y2 + 6] == 2) || (matrixbuff[x2 + 4][y2 + 5] == 2) || (matrixbuff[x2 + 4][y2 + 6] == 2) ||
          (matrixbuff[x2 + 5][y2 + 4] == 2) || (matrixbuff[x2 + 6][y2 + 4] == 2) || (matrixbuff[x2 + 6][y2 + 5] == 2) ||
          (matrixbuff[x2 + 6][y2 + 6] == 2)) {
        puts("\nSpacecraft collision with enemy 2 : GREEN");
        spacecraft_collision_2 = true;
        drawEnemy_new(x2, y2, 0);
        burst_animation(x2, y2, enemy_color_2);
        x2 = EXPIRED;
        y2 = EXPIRED;
      }
    }

    if (spacecraft_collision_1 != true) {
      if ((matrixbuff[x1][y1 + 2] == 2) || (matrixbuff[x1 + 1][y1 + 1] == 2) || (matrixbuff[x1 + 2][y1] == 2) ||
          (matrixbuff[x1 + 3][y1 - 1] == 2) || (matrixbuff[x1 + 4][y1] == 2) || (matrixbuff[x1 + 5][y1 + 1] == 2) ||
          (matrixbuff[x1 + 6][y1 + 2] == 2) || (matrixbuff[x1][y1 + 4] == 2) || (matrixbuff[x1][y1 + 5] == 2) ||
          (matrixbuff[x1][y1 + 6] == 2) || (matrixbuff[x1 + 1][y1 + 4] == 2) || (matrixbuff[x1 + 2][y1 + 5] == 2) ||
          (matrixbuff[x1 + 2][y1 + 6] == 2) || (matrixbuff[x1 + 4][y1 + 5] == 2) || (matrixbuff[x1 + 4][y1 + 6] == 2) ||
          (matrixbuff[x1 + 5][y1 + 4] == 2) || (matrixbuff[x1 + 6][y1 + 4] == 2) || (matrixbuff[x1 + 6][y1 + 5] == 2) ||
          (matrixbuff[x1 + 6][y1 + 6] == 2)) {
        puts("\nSpacecraft collision with enemy 1 : GREEN");
        spacecraft_collision_1 = true;
        drawEnemy_new(x1, y1, 0);
        burst_animation(x1, y1, enemy_color_1);
        x1 = EXPIRED;
        y1 = EXPIRED;
      }
    }
  } else if (health < 50 && health > 1) {

    // when spacecraft is red in color
    if (spacecraft_collision_2 != true) {
      if ((matrixbuff[x2][y2 + 2] == 4) || (matrixbuff[x2 + 1][y2 + 1] == 4) || (matrixbuff[x2 + 2][y2] == 4) ||
          (matrixbuff[x2 + 3][y2 - 1] == 4) || (matrixbuff[x2 + 4][y2] == 4) || (matrixbuff[x2 + 5][y2 + 1] == 4) ||
          (matrixbuff[x2 + 6][y2 + 2] == 4) || (matrixbuff[x2][y2 + 4] == 4) || (matrixbuff[x2][y2 + 5] == 4) ||
          (matrixbuff[x2][y2 + 6] == 4) || (matrixbuff[x2 + 1][y2 + 4] == 4) || (matrixbuff[x2 + 2][y2 + 5] == 4) ||
          (matrixbuff[x2 + 2][y2 + 6] == 4) || (matrixbuff[x2 + 4][y2 + 5] == 4) || (matrixbuff[x2 + 4][y2 + 6] == 4) ||
          (matrixbuff[x2 + 5][y2 + 4] == 4) || (matrixbuff[x2 + 6][y2 + 4] == 4) || (matrixbuff[x2 + 6][y2 + 5] == 4) ||
          (matrixbuff[x2 + 6][y2 + 6] == 4)) {
        puts("\nSpacecraft collision with enemy 2 : RED");
        spacecraft_collision_2 = true;
        drawEnemy_new(x2, y2, 0);
        burst_animation(x2, y2, enemy_color_2);
        x2 = EXPIRED;
        y2 = EXPIRED;
      }
    }

    if (spacecraft_collision_1 != true) {
      // spacecraft collision for enemy spaceship 1
      if ((matrixbuff[x1][y1 + 2] == 4) || (matrixbuff[x1 + 1][y1 + 1] == 4) || (matrixbuff[x1 + 2][y1] == 4) ||
          (matrixbuff[x1 + 3][y1 - 1] == 4) || (matrixbuff[x1 + 4][y1] == 4) || (matrixbuff[x1 + 5][y1 + 1] == 4) ||
          (matrixbuff[x1 + 6][y1 + 2] == 4) || (matrixbuff[x1][y1 + 4] == 4) || (matrixbuff[x1][y1 + 5] == 4) ||
          (matrixbuff[x1][y1 + 6] == 4) || (matrixbuff[x1 + 1][y1 + 4] == 4) || (matrixbuff[x1 + 2][y1 + 5] == 4) ||
          (matrixbuff[x1 + 2][y1 + 6] == 4) || (matrixbuff[x1 + 4][y1 + 5] == 4) || (matrixbuff[x1 + 4][y1 + 6] == 4) ||
          (matrixbuff[x1 + 5][y1 + 4] == 4) || (matrixbuff[x1 + 6][y1 + 4] == 4) || (matrixbuff[x1 + 6][y1 + 5] == 4) ||
          (matrixbuff[x1 + 6][y1 + 6] == 4)) {
        puts("\nSpacecraft collision with enemy 1 : RED");
        spacecraft_collision_1 = true;
        drawEnemy_new(x1, y1, 0);
        burst_animation(x1, y1, enemy_color_1);
        x1 = EXPIRED;
        y1 = EXPIRED;
      }

      // missile collision
    }
  }
}

void villianCollision(void) {

  for (int i = 0; i < bullet_count; i++) {

    // if y axis is less than 64
    if (bulletarray[i][1] < 64) {
      // if collision detected then turn off the bullet, else increment the bullet. Also turn off if greater than 64.
      if ((matrixbuff[bulletarray[i][0]][bulletarray[i][1] + 1]) != 0) {
        // puts("villian collision");
        bulletarray[i][1] = EXPIRED;
        villian_health--;
      } else {
        bulletarray[i][1]++;
        if (bulletarray[i][1] > 62)
          bulletarray[i][1] = EXPIRED;
      }
    }
  }

  if ((matrixbuff[villian_bullet1_pos_x][villian_bullet1_pos_y - 5] == 7) ||
      (matrixbuff[villian_bullet1_pos_x][villian_bullet1_pos_y - 4] == 7) ||
      (matrixbuff[villian_bullet1_pos_x][villian_bullet1_pos_y - 3] == 7)) {
    // puts("\n bullet 2 bullet collision");
    b2b_collision_1 = true;
    updatePixel(villian_bullet1_pos_x, villian_bullet1_pos_y - 1, 0);
    updatePixel(villian_bullet1_pos_x, villian_bullet1_pos_y, 0);
  }

  if ((matrixbuff[villian_bullet2_pos_x][villian_bullet2_pos_y - 5] == 7) ||
      (matrixbuff[villian_bullet2_pos_x][villian_bullet2_pos_y - 4] == 7) ||
      (matrixbuff[villian_bullet2_pos_x][villian_bullet2_pos_y - 3] == 7)) {
    // puts("\n bullet 2 bullet collision");
    b2b_collision_2 = true;
    updatePixel(villian_bullet2_pos_x, villian_bullet2_pos_y - 1, 0);
    updatePixel(villian_bullet2_pos_x, villian_bullet2_pos_y, 0);
  }

  if ((matrixbuff[villian_bullet3_pos_x][villian_bullet3_pos_y - 5] == 7) ||
      (matrixbuff[villian_bullet3_pos_x][villian_bullet3_pos_y - 4] == 7) ||
      (matrixbuff[villian_bullet3_pos_x][villian_bullet3_pos_y - 3] == 7)) {
    // puts("\n bullet 2 bullet collision");
    b2b_collision_3 = true;
    updatePixel(villian_bullet3_pos_x, villian_bullet3_pos_y - 1, 0);
    updatePixel(villian_bullet3_pos_x, villian_bullet3_pos_y, 0);
  }

  if ((matrixbuff[villian_bullet1_pos_x][villian_bullet1_pos_y - 3] == 2) ||
      (matrixbuff[villian_bullet1_pos_x][villian_bullet1_pos_y - 3] == 4)) {
    // puts("\n bullet1 2 spaceship collision");
    b2ship_collision1 = true;
    updatePixel(villian_bullet1_pos_x, villian_bullet1_pos_y - 1, 0);
    updatePixel(villian_bullet1_pos_x, villian_bullet1_pos_y, 0);
    health--;
    // printf("\nhealth 1:%d", health);
  }

  if ((matrixbuff[villian_bullet2_pos_x][villian_bullet2_pos_y - 3] == 2) ||
      (matrixbuff[villian_bullet2_pos_x][villian_bullet2_pos_y - 3] == 4)) {
    // puts("\n bullet2 2 spaceship collision");
    b2ship_collision2 = true;

    updatePixel(villian_bullet2_pos_x, villian_bullet2_pos_y - 1, 0);
    updatePixel(villian_bullet2_pos_x, villian_bullet2_pos_y, 0);

    // cur_time = time(NULL);
    // cur_t_string = ctime(&cur_time);
    // (void)printf("\n The Current time is : %s \n", cur_t_string);
    health--;
    // printf("\nhealth 2:%d", health);
  }

  if ((matrixbuff[villian_bullet3_pos_x][villian_bullet3_pos_y - 3] == 2) ||
      (matrixbuff[villian_bullet3_pos_x][villian_bullet3_pos_y - 3] == 4)) {
    // puts("\n bullet3 2 spaceship collision");
    b2ship_collision3 = true;
    updatePixel(villian_bullet3_pos_x, villian_bullet3_pos_y - 1, 0);
    updatePixel(villian_bullet3_pos_x, villian_bullet3_pos_y, 0);
    // cur_time = time(NULL);
    // cur_t_string = ctime(&cur_time);
    // (void)printf("\n The Current time is : %s \n", cur_t_string);
    health--;
    // printf("\nhealth 3:%d", health);
  }
}

#if 0

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
    puts("\nEnemy 2 bullet collision");
    bullet_collision_2 = true;
    kill_count++;
    drawEnemy_new(x2, y2, 0);
    burst_animation(x2, y2, enemy_color_2);
  }

  // bullet collision for enemy spaceship 1
  if ((matrixbuff[x1][y1 - 1] == 7) || (matrixbuff[x1 + 1][y1 - 1] == 7) || (matrixbuff[x1 + 2][y1 - 1] == 7) ||
      (matrixbuff[x1 + 3][y1 - 1] == 7) || (matrixbuff[x1 + 4][y1 - 1] == 7) || (matrixbuff[x1 + 5][y1 - 1] == 7) ||
      (matrixbuff[x1 + 6][y1 - 1] == 7)) {
    puts("\nEnemy 1 bullet collision");
    bullet_collision_1 = true;
    kill_count++;
    drawEnemy_new(x1, y1, 0);
    burst_animation(x1, y1, enemy_color_1);
  }

  if (health >= 50) {

    if (spacecraft_collision_2 != true) {
      if ((matrixbuff[x2][y2 + 2] == 2) || (matrixbuff[x2 + 1][y2 + 1] == 2) || (matrixbuff[x2 + 2][y2] == 2) ||
          (matrixbuff[x2 + 3][y2 - 1] == 2) || (matrixbuff[x2 + 4][y2] == 2) || (matrixbuff[x2 + 5][y2 + 1] == 2) ||
          (matrixbuff[x2 + 6][y2 + 2] == 2) || (matrixbuff[x2 + 1][y2 + 4] == 2) || (matrixbuff[x2 + 1][y2 + 5] == 2) ||
          (matrixbuff[x2 + 3][y2 + 4] == 2) || (matrixbuff[x2 + 3][y2 + 5] == 2) || (matrixbuff[x2 + 5][y2 + 4] == 2) ||
          (matrixbuff[x2 + 5][y2 + 5] == 2)) {
        puts("\nSpacecraft collision with enemy 2 : GREEN");
        spacecraft_collision_2 = true;
        drawEnemy_new(x2, y2, 0);
        burst_animation(x2, y2, enemy_color_2);
      }
    }

    if (spacecraft_collision_1 != true) {
      if ((matrixbuff[x1][y1 + 2] == 2) || (matrixbuff[x1 + 1][y1 + 1] == 2) || (matrixbuff[x1 + 2][y1] == 2) ||
          (matrixbuff[x1 + 3][y1 - 1] == 2) || (matrixbuff[x1 + 4][y1] == 2) || (matrixbuff[x1 + 5][y1 + 1] == 2) ||
          (matrixbuff[x1 + 6][y1 + 2] == 2) || (matrixbuff[x1 + 1][y1 + 4] == 2) || (matrixbuff[x1 + 1][y1 + 5] == 2) ||
          (matrixbuff[x1 + 3][y1 + 4] == 2) || (matrixbuff[x1 + 3][y1 + 5] == 2) || (matrixbuff[x1 + 5][y1 + 4] == 2) ||
          (matrixbuff[x1 + 5][y1 + 5] == 2)) {
        puts("\nSpacecraft collision with enemy 1 : GREEN");
        spacecraft_collision_1 = true;
        drawEnemy_new(x1, y1, 0);
        burst_animation(x1, y1, enemy_color_1);
      }
    }
  } else if (health < 50 && health > 1) {

    // spacecraft collision for enemy spaceship 2
    if (spacecraft_collision_2 != true) {
      if ((matrixbuff[x2][y2 + 2] == 4) || (matrixbuff[x2 + 1][y2 + 1] == 4) || (matrixbuff[x2 + 2][y2] == 4) ||
          (matrixbuff[x2 + 3][y2 - 1] == 4) || (matrixbuff[x2 + 4][y2] == 4) || (matrixbuff[x2 + 5][y2 + 1] == 4) ||
          (matrixbuff[x2 + 6][y2 + 2] == 4) || (matrixbuff[x2 + 1][y2 + 4] == 4) || (matrixbuff[x2 + 1][y2 + 5] == 4) ||
          (matrixbuff[x2 + 3][y2 + 4] == 4) || (matrixbuff[x2 + 3][y2 + 5] == 4) || (matrixbuff[x2 + 5][y2 + 4] == 4) ||
          (matrixbuff[x2 + 5][y2 + 5] == 4)) {
        puts("\nSpacecraft collision with enemy 2 : RED");
        spacecraft_collision_2 = true;
        drawEnemy_new(x2, y2, 0);
        burst_animation(x2, y2, enemy_color_2);
      }
    }

    if (spacecraft_collision_1 != true) {
      // spacecraft collision for enemy spaceship 1
      if ((matrixbuff[x1][y1 + 2] == 4) || (matrixbuff[x1 + 1][y1 + 1] == 4) || (matrixbuff[x1 + 2][y1] == 4) ||
          (matrixbuff[x1 + 3][y1 - 1] == 4) || (matrixbuff[x1 + 4][y1] == 4) || (matrixbuff[x1 + 5][y1 + 1] == 4) ||
          (matrixbuff[x1 + 6][y1 + 2] == 4) || (matrixbuff[x1 + 1][y1 + 4] == 4) || (matrixbuff[x1 + 1][y1 + 5] == 4) ||
          (matrixbuff[x1 + 3][y1 + 4] == 4) || (matrixbuff[x1 + 3][y1 + 5] == 4) || (matrixbuff[x1 + 5][y1 + 4] == 4) ||
          (matrixbuff[x1 + 5][y1 + 5] == 4)) {
        puts("\nSpacecraft collision with enemy 1 : RED");
        spacecraft_collision_1 = true;
        drawEnemy_new(x1, y1, 0);
        burst_animation(x1, y1, enemy_color_1);
      }
    }

    // missile collision
  }
}

#endif

// void checkCollision(void) {

//   // handle bullet movement and collision with enemy
//   for (int i = 0; i < bullet_count; i++) {

//     // if y axis is less than 64
//     if (bulletarray[i][1] != EXPIRED) {
//       // if collision detected then turn off the bullet, else increment the bullet. Also turn off if greater than 64.
//       if ((matrixbuff[bulletarray[i][0]][bulletarray[i][1] + 1]) != 0) {
//         bulletarray[i][1] = EXPIRED;
//       } else {
//         bulletarray[i][1]++;
//         if (bulletarray[i][1] > 64)
//           bulletarray[i][1] = EXPIRED;
//       }
//     }
//   }

//   // check if bullet hit enemy spaceship 2
//   if ((matrixbuff[x2][y2 - 1] == 7) || (matrixbuff[x2 + 1][y2 - 1] == 7) || (matrixbuff[x2 + 2][y2 - 1] == 7) ||
//       (matrixbuff[x2 + 3][y2 - 1] == 7) || (matrixbuff[x2 + 4][y2 - 1] == 7) || (matrixbuff[x2 + 5][y2 - 1] == 7) ||
//       (matrixbuff[x2 + 6][y2 - 1] == 7)) {
//     bullet_collision_2 = true;
//     drawEnemy_new(x2, y2, 0);
//     burst_animation(x2, y2, enemy_color_2);
//     // updateDisplay();
//     // updateDisplay();
//     // updateDisplay();
//     // updateDisplay();
//     // updateDisplay();
//     // delay__ms(4);
//     // burst_animation(x1, y1, 7);
//     // delay__ms(4);
//     // burst_animation(x1, y1, 7);
//     // delay__ms(4);
//     // burst_animation(x1, y1, 7);
//     // delay__ms(4);
//   }

//   // bullet collision for enemy spaceship 1
//   if ((matrixbuff[x1][y1 - 1] == 7) || (matrixbuff[x1 + 1][y1 - 1] == 7) || (matrixbuff[x1 + 2][y1 - 1] == 7) ||
//       (matrixbuff[x1 + 3][y1 - 1] == 7) || (matrixbuff[x1 + 4][y1 - 1] == 7) || (matrixbuff[x1 + 5][y1 - 1] == 7) ||
//       (matrixbuff[x1 + 6][y1 - 1] == 7)) {
//     bullet_collision_1 = true;
//     drawEnemy_new(x1, y1, 0);
//     burst_animation(x1, y1, enemy_color_1);
//     // updateDisplay();
//     // updateDisplay();
//     // updateDisplay();
//     // updateDisplay();
//     // updateDisplay();
//     // delay__ms(4);
//     // burst_animation(x1, y1, 7);
//     // delay__ms(4);
//     // burst_animation(x1, y1, 7);
//     // delay__ms(4);
//     // burst_animation(x1, y1, 7);
//     // delay__ms(4);
//   }

//   // spacecraft collision for enemy spaceship 2
//   if ((matrixbuff[x2][y2 + 2] == 4) || (matrixbuff[x2 + 1][y2 + 1] == 4) || (matrixbuff[x2 + 2][y2] == 4) ||
//       (matrixbuff[x2 + 3][y2 - 1] == 4) || (matrixbuff[x2 + 4][y2] == 4) || (matrixbuff[x2 + 5][y2 + 1] == 4) ||
//       (matrixbuff[x2 + 6][y2 + 2] == 4) || (matrixbuff[x2 + 1][y2 + 4] == 4) || (matrixbuff[x2 + 1][y2 + 5] == 4) ||
//       (matrixbuff[x2 + 3][y2 + 4] == 4) || (matrixbuff[x2 + 3][y2 + 5] == 4) || (matrixbuff[x2 + 5][y2 + 4] == 4) ||
//       (matrixbuff[x2 + 5][y2 + 5] == 4)) {
//     spacecraft_collision_2 = true;
//     drawEnemy_new(x2, y2, 0);
//     burst_animation(x2, y2, enemy_color_2);
//     // updateDisplay();
//     // updateDisplay();
//     // updateDisplay();
//   } else if ((matrixbuff[x2][y2 + 2] == 2) || (matrixbuff[x2 + 1][y2 + 1] == 2) || (matrixbuff[x2 + 2][y2] == 2) ||
//              (matrixbuff[x2 + 3][y2 - 1] == 2) || (matrixbuff[x2 + 4][y2] == 2) || (matrixbuff[x2 + 5][y2 + 1] == 2)
//              || (matrixbuff[x2 + 6][y2 + 2] == 2) || (matrixbuff[x2 + 1][y2 + 4] == 2) || (matrixbuff[x2 + 1][y2 + 5]
//              == 2) || (matrixbuff[x2 + 3][y2 + 4] == 2) || (matrixbuff[x2 + 3][y2 + 5] == 2) || (matrixbuff[x2 +
//              5][y2 + 4] == 2) || (matrixbuff[x2 + 5][y2 + 5] == 2)) {
//     spacecraft_collision_2 = true;
//     drawEnemy_new(x2, y2, 0);
//     burst_animation(x2, y2, enemy_color_2);
//     // updateDisplay();
//     // updateDisplay();
//     // updateDisplay();
//   }

//   // spacecraft collision for enemy spaceship 1
//   if ((matrixbuff[x1][y1 + 2] == 4) || (matrixbuff[x1 + 1][y1 + 1] == 4) || (matrixbuff[x1 + 2][y1] == 4) ||
//       (matrixbuff[x1 + 3][y1 - 1] == 4) || (matrixbuff[x1 + 4][y1] == 4) || (matrixbuff[x1 + 5][y1 + 1] == 4) ||
//       (matrixbuff[x1 + 6][y1 + 2] == 4) || (matrixbuff[x1 + 1][y1 + 4] == 4) || (matrixbuff[x1 + 1][y1 + 5] == 4) ||
//       (matrixbuff[x1 + 3][y1 + 4] == 4) || (matrixbuff[x1 + 3][y1 + 5] == 4) || (matrixbuff[x1 + 5][y1 + 4] == 4) ||
//       (matrixbuff[x1 + 5][y1 + 5] == 4)) {
//     spacecraft_collision_1 = true;
//     drawEnemy_new(x1, y1, 0);
//     burst_animation(x1, y1, enemy_color_1);
//     // updateDisplay();
//     // updateDisplay();
//     // updateDisplay();
//   } else if ((matrixbuff[x1][y1 + 2] == 2) || (matrixbuff[x1 + 1][y1 + 1] == 2) || (matrixbuff[x1 + 2][y1] == 2) ||
//              (matrixbuff[x1 + 3][y1 - 1] == 2) || (matrixbuff[x1 + 4][y1] == 2) || (matrixbuff[x1 + 5][y1 + 1] == 2)
//              || (matrixbuff[x1 + 6][y1 + 2] == 2) || (matrixbuff[x1 + 1][y1 + 4] == 2) || (matrixbuff[x1 + 1][y1 + 5]
//              == 2) || (matrixbuff[x1 + 3][y1 + 4] == 2) || (matrixbuff[x1 + 3][y1 + 5] == 2) || (matrixbuff[x1 +
//              5][y1 + 4] == 2) || (matrixbuff[x1 + 5][y1 + 5] == 2)) {
//     spacecraft_collision_1 = true;
//     drawEnemy_new(x1, y1, 0);
//     burst_animation(x1, y1, enemy_color_1);
//     // updateDisplay();
//     // updateDisplay();
//     // updateDisplay();
//   }

//   // missile collision
// }

// void checkCollision(void) {

//   // bullet collision
//   for (int i = 0; i < bullet_count; i++) {

//     if (bulletarray[i][1] != EXPIRED) {

//       if ((matrixbuff[bulletarray[i][0]][bulletarray[i][1] + 1]) != 0) {
//         // bullet_collision = true;
//         bulletarray[i][1] = EXPIRED;
//       } else {
//         bulletarray[i][1]++;
//         if (bulletarray[i][1] > 64)
//           bulletarray[i][1] = EXPIRED;
//       }
//     }
//   }

//   // bullet collision for enemy spaceship 2
//   if ((matrixbuff[x2][y2 - 1] == 7) || (matrixbuff[x2 + 1][y2 - 1] == 7) || (matrixbuff[x2 + 2][y2 - 1] == 7) ||
//       (matrixbuff[x2 + 3][y2 - 1] == 7) || (matrixbuff[x2 + 4][y2 - 1] == 7) || (matrixbuff[x2 + 5][y2 - 1] == 7) ||
//       (matrixbuff[x2 + 6][y2 - 1] == 7)) {
//     bullet_collision_2 = true;
//     drawEnemy_new(x2, y2, 0);
//   }

//   // spacecraft collision for enemy spaceship 2
//   if ((matrixbuff[x2][y2 + 2] == 4) || (matrixbuff[x2 + 1][y2 + 1] == 4) || (matrixbuff[x2 + 2][y2] == 4) ||
//       (matrixbuff[x2 + 3][y2 - 1] == 4) || (matrixbuff[x2 + 4][y2] == 4) || (matrixbuff[x2 + 5][y2 + 1] == 4) ||
//       (matrixbuff[x2 + 6][y2 + 2] == 4) || (matrixbuff[x2 + 1][y2 + 4] == 4) || (matrixbuff[x2 + 1][y2 + 5] == 4) ||
//       (matrixbuff[x2 + 3][y2 + 4] == 4) || (matrixbuff[x2 + 3][y2 + 5] == 4) || (matrixbuff[x2 + 5][y2 + 4] == 4) ||
//       (matrixbuff[x2 + 5][y2 + 5] == 4)) {
//     spacecraft_collision_2 = true;
//     drawEnemy_new(x2, y2, 0);
//   }

//   // bullet collision for enemy spaceship 1
//   if ((matrixbuff[x1][y1 - 1] == 7) || (matrixbuff[x1 + 1][y1 - 1] == 7) || (matrixbuff[x1 + 2][y1 - 1] == 7) ||
//       (matrixbuff[x1 + 3][y1 - 1] == 7) || (matrixbuff[x1 + 4][y1 - 1] == 7) || (matrixbuff[x1 + 5][y1 - 1] == 7) ||
//       (matrixbuff[x1 + 6][y1 - 1] == 7)) {
//     bullet_collision_1 = true;
//     drawEnemy_new(x1, y1, 0);
//   }
//   // spacecraft collision for enemy spaceship 1
//   if ((matrixbuff[x1][y1 + 2] == 4) || (matrixbuff[x1 + 1][y1 + 1] == 4) || (matrixbuff[x1 + 2][y1] == 4) ||
//       (matrixbuff[x1 + 3][y1 - 1] == 4) || (matrixbuff[x1 + 4][y1] == 4) || (matrixbuff[x1 + 5][y1 + 1] == 4) ||
//       (matrixbuff[x1 + 6][y1 + 2] == 4) || (matrixbuff[x1 + 1][y1 + 4] == 4) || (matrixbuff[x1 + 1][y1 + 5] == 4) ||
//       (matrixbuff[x1 + 3][y1 + 4] == 4) || (matrixbuff[x1 + 3][y1 + 5] == 4) || (matrixbuff[x1 + 5][y1 + 4] == 4) ||
//       (matrixbuff[x1 + 5][y1 + 5] == 4)) {
//     spacecraft_collision_1 = true;
//     drawEnemy_new(x1, y1, 0);
//   }

//   // missile collision
// }