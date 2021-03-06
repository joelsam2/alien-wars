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

#define DEBUG 1
#define RECEIVER

extern joystick joystick_control_signal;

TaskHandle_t e1, e2, villain;

void refreshDisplayTask() {

  vTaskSuspend(villain);

  while (1) {

    if (kill_count > 10 && level == 2) {

      drawSpaceship(ship_position_x, ship_position_y, 0);
      clearDisplay();
      level_up_screen(3);
      level = 3;
      clearDisplay();
    } else if (kill_count > 5 && level == 1) {

      drawSpaceship(ship_position_x, ship_position_y, 0);
      clearDisplay();
      level_up_screen(2);
      level = 2;
      clearDisplay();
    } else if (level == 3) {

      vTaskSuspend(e1);
      vTaskSuspend(e2);
      vTaskResume(villain);
    }

    if (health >= 50) {
      drawSpaceship(ship_position_x, ship_position_y, 2);
    } else if (health < 50 && health > 1) {
      drawSpaceship(ship_position_x, ship_position_y, 4);
    } else {
      // game over !!
      game_over_screen();
    }

    // drawEnemy(x1, y1, enemy_color_1);
    // drawEnemy(x2, y2, enemy_color_2);

    if (level != 3) {
      drawEnemy_new(x1, y1, enemy_color_1);
      drawEnemy_new(x2, y2, enemy_color_2);
    }

    // burst_animation(x1, y1, enemy_color_2);

    updateDisplay();

    checkCollision();

    // if (bullet_collision_1 == true || spacecraft_collision_1 == true) {
    //   // drawEnemy(x1, y1, 0);
    // }
    // if (bullet_collision_2 == true || spacecraft_collision_2 == true) {
    //   // drawEnemy(x2, y2, 0);
    // }

    clearDisplay();
    vTaskDelay(3); // refresh rate 3ms tested for all possible scenarios
  }
}

void fireTask() {

  while (1) {

    if (shootbool) {
      // delay__ms(30);
      firing_position();

      PLAY_BULLET();

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
      // puts("\nshootbool");
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
    enemy_color_1 = (1 + rand() % 6);

    for (y1 = 57; y1 > 0; y1--) {

      if (level == 1) {
        delay__ms(50);
      } else if (level == 2) {
        delay__ms(40);
      } else if (level == 2) {
        delay__ms(30);
      }

      if (bullet_collision_1 == true || spacecraft_collision_1 == true) {

        PLAY_COLLISION();
        break;
      }
    }

    if (bullet_collision_1) {
      kill_count++;
      bullet_collision_1 = false;
    } else if (spacecraft_collision_1) {
      health -= 10;
      spacecraft_collision_1 = false;
    }
  }
}

void enemyTask2() {

  while (1) {

    x2 = 16 + (rand() % 10);
    enemy_color_2 = (1 + rand() % 6);
    int temp = (rand() % 10);
    for (y2 = 57; y2 > 0; y2--) {

      if (level == 1) {
        delay__ms(50);
      } else if (level == 2) {
        delay__ms(40);
      } else if (level == 2) {
        delay__ms(30);
      }

      if (bullet_collision_2 == true || spacecraft_collision_2 == true) {

        PLAY_COLLISION();
        break;
      }
    }

    if (bullet_collision_2) {
      kill_count++;
      bullet_collision_2 = false;
    } else if (spacecraft_collision_2) {
      health -= 10;
      spacecraft_collision_2 = false;
    }
  }
}

void villainTask() {

  while (1) {

    drawVillain(14, 45, 4);
  }
}

#if 0

bool enemy_boundary(uint8_t x, uint8_t y, uint8_t color)
{
   if( (matrixbuff[x2][y2 - 1] == 4) || 
   (matrixbuff[x2][y2 - 1] == 4) || (matrixbuff[x2][y2 - 1] == 5  
  



}


void shoot_isr() {
  shootbool = 1;
  // xSemaphoreGiveFromISR(shoot, NULL);
  LPC_GPIOINT->IO0IntClr |= (1 << 30);
  // portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

bool uart_put(char output_byte) {
  uint8_t THRE = 5;
  if (LPC_UART3->LSR & (1 << THRE)) {
    LPC_UART3->THR = output_byte;
    while (!(LPC_UART3->LSR & (1 << THRE)))
      ;
  }
  return true;
}

#endif

int main(void) {

  // It is advised to either run the uart_task, or the SJ2 command-line (CLI), but not both
  // Change '#if 0' to '#if 1' and vice versa to try it out
#if 0
  // printf() takes more stack space, size this tasks' stack higher
  xTaskCreate(uart_task, "uart", (512U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
#else
  sj2_cli__init();
  // UNUSED(uart_task); // uart_task is un-used in if we are doing cli init()
#endif

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

  // shoot = xSemaphoreCreateBinary();

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

#if 0
  LPC_GPIOINT->IO0IntEnR |= (1 << 30);

  NVIC_EnableIRQ(GPIO_IRQn);
#endif

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

  gpio_set(2, 2, 0);  // background music trigger
  gpio_set(2, 0, 0);  // are you ready! trigger
  gpio_set(1, 28, 0); // bullet sound trigger
  gpio_set(1, 29, 0); // collision sound trigger

  // trigger to play background music
  PLAY_BACKGROUNDMZK();

  puts("Starting RTOS");

  // while (1) {
  //   burst_animation(16, 32, 7);

  //   drawEnemy_new(16, 50, 7);
  //   updateDisplay();
  // }

  // gpio_set(2, 0, 1);

  // while (1) {
  //   if (gpio_get(2, 0)) {
  //     printf("1\r\n");
  //   } else {
  //     printf("0\r\n");
  //   }
  // }

#if DEBUG

#else
  (void)startup_screen();
#endif

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