#include "FreeRTOS.h"
#include "alien_uart.h"
#include "board_io.h"
#include "common_macros.h"
#include "gpio.h"
#include "gpio_driver.h"
#include "i2c.h"
#include "led_matrix.h"
#include "mp3_encoder.h"
#include "portmacro.h"
#include "semphr.h"
#include "sj2_cli.h"
#include "startup_sequence.h"
#include "task.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void checkCollision(void);

#define DEBUG 0

extern joystick joystick_control_signal;

TaskHandle_t e1, e2, villain;

SemaphoreHandle_t health_mutex;

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
        x2 = EXPIRED;
        y2 = EXPIRED;

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
        x1 = EXPIRED;
        y1 = EXPIRED;

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
        x2 = EXPIRED;
        y2 = EXPIRED;

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
        x1 = EXPIRED;
        y1 = EXPIRED;

        burst_animation(x1, y1, enemy_color_1);
      }

      // missile collision
    }
  }
}

void refreshDisplayTask() {

  vTaskSuspend(villain);

  while (1) {

    if (level < 3) {
      if (bullet_collision_1 == false && spacecraft_collision_1 == false) {
        drawEnemy_new(x1, y1, enemy_color_1);
      }

      if (bullet_collision_2 == false && spacecraft_collision_2 == false) {
        drawEnemy_new(x2, y2, enemy_color_2);
      }
    }

    if (health >= 50) {
      drawSpaceship(ship_position_x, ship_position_y, 2); // green
    } else if (health < 50 && health > 1) {
      drawSpaceship(ship_position_x, ship_position_y, 4); // red
    } else {
      // game over !!
      game_over_screen();
    }

    if (kill_count > 5 && level == 1) {
      puts("Enter level 1");
      drawSpaceship(ship_position_x, ship_position_y, 0);
      clearDisplay();
      level_up_screen(2);
      level = 2;
      clearDisplay();
    } else if (kill_count > 10 && level == 2) {
      puts("Enter level 2");
      drawSpaceship(ship_position_x, ship_position_y, 0);
      clearDisplay();
      level_up_screen(3);
      level = 3;
      clearDisplay();
    } else if (level == 3) {
      puts("Enter level 3");
      vTaskSuspend(e1);
      vTaskSuspend(e2);
      vTaskResume(villain);
      level = 4;
    }

    checkCollision();

    updateDisplay();

    clearDisplay();

    vTaskDelay(3); // refresh rate 3ms tested for all possible scenarios
  }
}

void fireTask() {

  while (1) {

    if (shootbool) {

      firing_position();

      // PLAY_BULLET();
      // play collision music
      i2c__write_single(2, 0x46, 0x09, 0x03);

      bulletarray[index1][index2++] = firing_position_x;
      bulletarray[index1++][index2] = firing_position_y;
      bullet_count++;

      if (index1 == 100) {
        index1 = 0;
      }
      if (index2 == 1) {
        index2 = 0;
      }
      if (bullet_count == 100) {
        bullet_count = 0;
      }

      shootbool = false;
    }

    for (x3 = 0; x3 < bullet_count; x3++) {

      if (bulletarray[x3][1] < 64) {

        updatePixel(bulletarray[x3][0], bulletarray[x3][1], 7);
        updatePixel(bulletarray[x3][0], ++bulletarray[x3][1], 7);
      }
    }

    vTaskDelay(30);
  }
}

void updateSpaceshipTask() {

  while (1) {

    (void)bluetooth_receive_data();

    // down
    if (((joystick_control_signal == 1) || (joystick_control_signal == 8) || (joystick_control_signal == 6)) &&
        (ship_position_x < 23)) {

      ship_position_x++;
    }

    // up
    if (((joystick_control_signal == 2) || (joystick_control_signal == 7) || (joystick_control_signal == 5)) &&
        (ship_position_x > 0)) {

      ship_position_x--;
    }

    // forward
    if (((joystick_control_signal == 4) || (joystick_control_signal == 5) || (joystick_control_signal == 6)) &&
        (ship_position_y < 59)) {

      ship_position_y++;
    }

    // backward
    if (((joystick_control_signal == 3) || (joystick_control_signal == 7) || (joystick_control_signal == 8)) &&
        (ship_position_y > 0)) {

      ship_position_y--;
    }

    // fire bullet
    if (joystick_control_signal == 9) {
      shootbool = true;
    }

    // on board fire bullet button
    if (gpio_get(0, 29)) {
      delay__ms(20);
      if (gpio_get(0, 29))
        shootbool = true;
    }

    // missile

    vTaskDelay(40);
  }
}

void enemyTask1() {

  while (1) {

    x1 = 0 + (rand() % 9); // 32/2 = 16-3 = 13
    y1 = 57;
    enemy_color_1 = (1 + rand() % 6);
    spacecraft_collision_1 = false;
    bullet_collision_1 = false;

    for (y1 = 57; y1 > 0; y1--) {

      if (level == 1) {
        delay__ms(50);
      } else if (level == 2) {
        delay__ms(30);
      }

      if ((bullet_collision_1 == true || spacecraft_collision_1 == true) && (x1 == EXPIRED && y1 == EXPIRED)) {
        // PLAY_COLLISION();
        // play collision music
        i2c__write_single(2, 0x46, 0x09, 0x04);
        if (bullet_collision_1 == true) {
          bullet_collision_1 = false;
        } else if (spacecraft_collision_1 == true) {
          spacecraft_collision_1 = false;
          health -= 10;
        }
        break;
      }
    }
  }
}

void enemyTask2() {

  while (1) {

    x2 = 16 + (rand() % 10);
    y2 = 57;
    enemy_color_2 = (1 + rand() % 6);
    int temp = (rand() % 10);

    spacecraft_collision_1 = false;
    bullet_collision_1 = false;

    for (y2 = 57; y2 > 0; y2--) {

      if (level == 1) {
        delay__ms(50);
      } else if (level == 2) {
        delay__ms(30);
      }

      if ((bullet_collision_2 == true || spacecraft_collision_2 == true) && (x2 == EXPIRED && y2 == EXPIRED)) {
        // PLAY_COLLISION();
        // play collision music
        i2c__write_single(2, 0x46, 0x09, 0x04);
        if (bullet_collision_2 == true) {
          bullet_collision_2 = false;
        } else if (spacecraft_collision_2 == true) {
          spacecraft_collision_2 = false;
          health -= 10;
        }
        break;
      }
    }
  }
}

void villainTask() {

  while (1) {

    drawVillain(14, 45, 4);
  }
}

void i2c_isr(void);

void i2c_isr(void) { gpio_set(2, 3, 1); }

void i2c2_init(void) {
  // i2c 2
  gpio_dir(0, 10, 0);
  LPC_IOCON->P0_10 = 0x02;

  gpio_dir(0, 11, 0);
  LPC_IOCON->P0_11 = 0x02;

  LPC_SC->PCLKSEL = 1;

  lpc_peripheral__turn_on_power_to(LPC_PERIPHERAL__I2C2);

  LPC_I2C2->MASK0 = 0x00;
  LPC_I2C2->MASK1 = 0x00;
  LPC_I2C2->MASK2 = 0x00;
  LPC_I2C2->MASK3 = 0x00;

  LPC_I2C2->ADR0 = 0x00;
  LPC_I2C2->ADR1 = 0x00;
  LPC_I2C2->ADR2 = 0x00;
  LPC_I2C2->ADR3 = 0x00;

  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__I2C2, i2c_isr);

  // set to master
  LPC_I2C2->CONSET = 0x40;
}

int main(void) {

  sj2_cli__init();

  health_mutex = xSemaphoreCreateMutex();

  ship_position_x = 0;
  ship_position_y = 0;
  position.enemy_count = 0;
  index1 = 0;
  index2 = 0;
  bullet_count = 0;
  shootbool = false;

  level = 1;

  kill_count = 0;
  health = 100;

  bullet_collision_1 = false;
  spacecraft_collision_1 = false;
  bullet_collision_2 = false;
  spacecraft_collision_2 = false;

  init_led_matrix();

  LPC_IOCON->P2_3 = 0;
  gpio_dir(2, 3, 0);

  LPC_IOCON->P0_29 = 0;
  gpio_dir(0, 29, 1);

  LPC_IOCON->P0_30 = 0;
  gpio_dir(0, 30, 1);

  LPC_IOCON->P1_15 = 0;
  gpio_dir(1, 15, 1);

  LPC_IOCON->P1_19 = 0;
  gpio_dir(1, 19, 1);

  uart__init_alien_uart3_receiver(LPC_PERIPHERAL__UART3, 96000000, 38400);

  gpio_dir(2, 2, 0);
  LPC_IOCON->P2_2 = 0x00;

  gpio_dir(2, 0, 0);
  LPC_IOCON->P2_0 = 0x00;

  gpio_dir(1, 28, 0);
  LPC_IOCON->P1_28 = 0x00;

  gpio_dir(1, 29, 0);
  LPC_IOCON->P1_29 = 0x00;

  gpio_set(2, 2, 0);  // background music trigger
  gpio_set(2, 0, 0);  // are you ready! trigger
  gpio_set(1, 28, 0); // bullet sound trigger
  gpio_set(1, 29, 0); // collision sound trigger

  // trigger to play background music
  // PLAY_BACKGROUNDMZK();
  // gpio_set(2, 2, 1);
  // delay__ms(10);
  // gpio_set(2, 2, 0);

  // play background music
  i2c__write_single(2, 0x46, 0x09, 0x01);

  puts("Starting RTOS");

  // gpio_set(2, 0, 1);

  // #ifdef DEBUG

  // #elif
  (void)startup_screen();
  // #endif

  srand(time(0));

  xTaskCreate(refreshDisplayTask, "refreshDisplayTask", 2048, NULL, 3, NULL);
  xTaskCreate(enemyTask1, "enemyTask1", 2048, NULL, 1, &e1);
  xTaskCreate(enemyTask2, "enemyTask2", 2048, NULL, 1, &e2);
  xTaskCreate(updateSpaceshipTask, "updateSpaceshipTask", 2048, NULL, 1, NULL);
  xTaskCreate(fireTask, "fireTask", 2048, NULL, 1, NULL);
  xTaskCreate(villainTask, "villainTask", 2048, NULL, 1, &villain);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}