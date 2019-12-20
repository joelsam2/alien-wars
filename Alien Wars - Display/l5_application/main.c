#include "FreeRTOS.h"
#include "alien_uart.h"
#include "board_io.h"
#include "common_macros.h"
#include "gpio.h"
#include "gpio_driver.h"
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

// MACRO ====================================================

#define DEBUG 0
#define RECEIVER

// GLOBAL VARIABLES =========================================

extern joystick joystick_control_signal;

TaskHandle_t e1, e2, villain;

// TASK =====================================================

void refreshDisplayTask() {

  vTaskSuspend(villain);

  while (1) {

    // puts("\nContext Switch");

    // check level, display villain only if level == 3
    if (level < 3) {

      drawEnemy_new(x1, y1, enemy_color_1);
      drawEnemy_new(x2, y2, enemy_color_2);
    } else {

      if (villian_health >= 25) {
        drawVillain(villian_position_x, villian_position_y, 4);
      } else if (villian_health < 25 && villian_health > 1) {
        drawVillain(villian_position_x, villian_position_y, 2);
      } else {
        // Victory animation required
        winner();
      }
    }

    // check health of spaceship, display in green or red accordingly
    if (health >= 50) {
      drawSpaceship(ship_position_x, ship_position_y, 2); // green
    } else if (health < 50 && health > 1) {
      drawSpaceship(ship_position_x, ship_position_y, 4); // red
    } else {
      // game over !!
      game_over_screen();
    }

    // check kill count and advance to next level
    if (kill_count > 10 && level == 1) {
      puts("Enter level 2");
      drawSpaceship(ship_position_x, ship_position_y, 0);
      clearDisplay();
      level_up_screen(2);
      level = 2;
      clearDisplay();
    } else if (kill_count > 20 && level == 2) {
      puts("Enter level 3");
      drawSpaceship(ship_position_x, ship_position_y, 0);
      clearDisplay();
      level_up_screen(3);
      level = 3;
      play(BACKGROUND);
      clearDisplay();
      vTaskSuspend(e1);
      vTaskSuspend(e2);
      clearDisplay();
      updateDisplay();
      vTaskResume(villain);
      health = 100;
    }

    // check if theres a collision in the matrix
    if (level < 3) {
      checkCollision();
    } else {
      // level == 3
      villianCollision();
    }

    // update the display according to all the paramters checked before
    updateDisplay();

    // clear the display to update new values
    clearDisplay();

    vTaskDelay(3); // refresh rate 3ms tested for all possible scenarios
  }
}

// TASK =====================================================

void fireTask() {

  while (1) {

    if (shootbool) {

      // delay__ms(30);
      firing_position();

      printf("bullet fire before\r\n");

      if (level < 3) {
        play(BULLET);
      }

      // play(BULLET);

      printf("bullet fire after\r\n");

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

        // bulletarray[x3][1]++;
      }
    }

    vTaskDelay(30);
  }
}

// TASK =====================================================

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

        play(COLLISION);
        printf("in enemny task 1 health = %d", health);
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

// TASK =====================================================

void enemyTask2() {

  while (1) {

    x2 = 16 + (rand() % 9);
    y2 = 57;
    enemy_color_2 = (1 + rand() % 6);

    spacecraft_collision_2 = false;
    bullet_collision_2 = false;

    for (y2 = 57; y2 > 0; y2--) {

      if (level == 1) {
        delay__ms(50);
      } else if (level == 2) {
        delay__ms(30);
      }

      if ((bullet_collision_2 == true || spacecraft_collision_2 == true) && (x2 == EXPIRED && y2 == EXPIRED)) {

        play(COLLISION);
        printf("in enemny task 2 health = %d", health);
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

// TASK =====================================================

void villainTask() {

  while (1) {

    villian_bullet1_pos_x = villian_position_x;
    villian_bullet1_pos_y = villian_position_y;

    villian_bullet2_pos_x = villian_position_x + 9;
    villian_bullet2_pos_y = villian_position_y;

    villian_bullet3_pos_x = villian_position_x - 9;
    villian_bullet3_pos_y = villian_position_y;

    if (villian_position_x == 18) {
      villian_inc_flag = false;
    }
    if (villian_position_x == 13) {
      villian_inc_flag = true;
    }

    if (villian_inc_flag)
      villian_position_x++;
    else
      villian_position_x--;

    for (int column_number = villian_position_y; column_number > 0; column_number--) {

      if (b2b_collision_1 == false && b2ship_collision1 == false) {
        updatePixel(villian_bullet1_pos_x, villian_bullet1_pos_y, 7);
        updatePixel(villian_bullet1_pos_x, --villian_bullet1_pos_y, 7);
      }

      if (b2b_collision_2 == false && b2ship_collision2 == false) {
        updatePixel(villian_bullet2_pos_x, villian_bullet2_pos_y, 7);
        updatePixel(villian_bullet2_pos_x, --villian_bullet2_pos_y, 7);
      }

      if (b2b_collision_3 == false && b2ship_collision3 == false) {
        updatePixel(villian_bullet3_pos_x, villian_bullet3_pos_y, 7);
        updatePixel(villian_bullet3_pos_x, --villian_bullet3_pos_y, 7);
      }

      delay__ms(20);
    }

    if (b2b_collision_1 == true)
      b2b_collision_1 = false;

    if (b2b_collision_2 == true)
      b2b_collision_2 = false;

    if (b2b_collision_3 == true)
      b2b_collision_3 = false;

    if (b2ship_collision1 == true)
      b2ship_collision1 = false;

    if (b2ship_collision2 == true)
      b2ship_collision2 = false;

    if (b2ship_collision3 == true)
      b2ship_collision3 = false;
  }
}

#if 0
void villainTask() {

  while (1) {
    villian_bullet1_pos_x = villian_position_x;
    villian_bullet2_pos_x = villian_position_x + 9;
    villian_bullet3_pos_x = villian_position_x - 9;

    // villian_bullet1_pos_y = villian_position_y;
    villian_bullet2_pos_y = villian_position_y + 4;
    villian_bullet3_pos_y = villian_position_y + 4;

    // for (int column_number = villian_position_y; column_number > 0; column_number--) {
    for (villian_bullet1_pos_y = villian_position_y; villian_bullet1_pos_y > 0; villian_bullet1_pos_y--) {

      // printf("\nvillian_bullet1_pos_y : %d", villian_bullet1_pos_y);
      // printf("\nmatrixbuff: %d", matrixbuff[villian_bullet1_pos_x][villian_bullet1_pos_y - 3]);
      // if (b2b_collision_1 == false) {
      // if (!(matrixbuff[villian_bullet1_pos_x][villian_bullet1_pos_y - 3] == 7)) {
      // puts("\nb2b");
      updatePixel(villian_bullet1_pos_x, villian_bullet1_pos_y, 7);
      updatePixel(villian_bullet1_pos_x, --villian_bullet1_pos_y, 7);
      // b2b_collision_1 = false;
      //}

      // updatePixel(villian_bullet2_pos_x, villian_bullet2_pos_y, 7);
      // updatePixel(villian_bullet2_pos_x, --villian_bullet2_pos_y, 7);

      // updatePixel(villian_bullet3_pos_x, villian_bullet3_pos_y, 7);
      // updatePixel(villian_bullet3_pos_x, --villian_bullet3_pos_y, 7);

      delay__ms(20);
    }
  }
}
#endif

// TASK =====================================================

void updateSpaceshipTask() {

  while (1) {

    (void)bluetooth_receive_data();

    // down
    if (((joystick_control_signal == 1) || (joystick_control_signal == 8) || (joystick_control_signal == 6)) &&
        (ship_position_x < 22)) {

      ship_position_x++;
    }

    // up
    if (((joystick_control_signal == 2) || (joystick_control_signal == 7) || (joystick_control_signal == 5)) &&
        (ship_position_x > 0)) {

      ship_position_x--;
    }

    // forward
    if (level < 3) {
      if (((joystick_control_signal == 4) || (joystick_control_signal == 5) || (joystick_control_signal == 6)) &&
          (ship_position_y < 59)) {
        ship_position_y++;
      }
    }

    if (level == 3) {
      if (((joystick_control_signal == 4) || (joystick_control_signal == 5) || (joystick_control_signal == 6)) &&
          (ship_position_y < 37)) {
        ship_position_y++;
      }
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
    // if (gpio_get(0, 29)) {
    //   delay__ms(20);
    //   if (gpio_get(0, 29))
    //     shootbool = true;
    // }

    // missile

    vTaskDelay(40);
  }
}

// MAIN =====================================================

int main(void) {

#if 0
      // printf() takes more stack space, size this tasks' stack higher
      xTaskCreate(uart_task, "uart", (512U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
#else
  sj2_cli__init();
  // UNUSED(uart_task); // uart_task is un-used in if we are doing cli init()
#endif

  configuration_init();

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

#ifdef RECEIVER
  uart__init_alien_uart3_receiver(LPC_PERIPHERAL__UART3, 96000000, 38400);
  // uart__init_alien_uart3_receiver(LPC_PERIPHERAL__UART3, 96000000, 9600);
  // xTaskCreate(bluetooth_receive, "bluetooth_receive", 1024U, NULL, 3, NULL);
#endif

  gpio_dir(2, 2, 0);
  LPC_IOCON->P2_2 = 0x00;

  gpio_dir(2, 0, 0);
  LPC_IOCON->P2_0 = 0x00;

  gpio_dir(1, 28, 0);
  LPC_IOCON->P1_28 = 0x00;

  gpio_dir(1, 29, 0);
  LPC_IOCON->P1_29 = 0x00;

  puts("Starting RTOS");

#if DEBUG

  level = 1;

#else

  // trigger to play background music
  play(BACKGROUND);

  (void)startup_screen();

#endif

  srand(time(0));

  xTaskCreate(refreshDisplayTask, "refreshDisplayTask", 2048, NULL, 3, NULL);
  xTaskCreate(enemyTask1, "enemyTask1", 2048, NULL, 1, &e1);
  xTaskCreate(enemyTask2, "enemyTask2", 2048, NULL, 1, &e2);
  xTaskCreate(updateSpaceshipTask, "updateSpaceshipTask", 2048, NULL, 1, NULL);
  xTaskCreate(fireTask, "fireTask", 2048, NULL, 2, NULL);
  xTaskCreate(villainTask, "villainTask", 2048, NULL, 1, &villain);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}