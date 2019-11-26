#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "gpio.h"
#include "sj2_cli.h"

#include "gpio_driver.h"
#include "led_matrix.h"

#include "portmacro.h"
#include "semphr.h"
#include <string.h>
#define MAX_DELAY 0xFFFF
SemaphoreHandle_t shoot;
// static signed BaseType_t xHigherPriorityTaskWoken ;
uint8_t ship_position_x, ship_position_y, firing_position_x, firing_position_y;
uint8_t index1 = 0;
uint8_t index2 = 0;
uint8_t bullet_count = 0;
uint8_t bulletarray[100][2];
bool spaceship_down = false;
bool spaceship_up = false;
bool spaceship_left = false;
bool spaceship_right = false;
bool shootbool = false;

void firing_position() {

  firing_position_x = ship_position_x + 4;
  firing_position_y = ship_position_y + 5;
}

void refreshDisplayTask() {

  while (1) {

    updateDisplay();
    vTaskDelay(6);
  }
}

void fireTask() {

  while (1) {
    if (shootbool) {
      // while ((gpio_get(0, 29)) && (gpio_get(0, 30)))
      //;

      firing_position();
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
    }
    // clearDisplay();
    // drawSpaceship(ship_position_x, ship_position_y, 4);
    for (int i = 0; i < bullet_count; i++) {
      if (bulletarray[i][1] < 64) {
        clearDisplay(); ////
        updatePixel(bulletarray[i][0], bulletarray[i][1], 7);
        updatePixel(bulletarray[i][0], ++bulletarray[i][1], 7);
        bulletarray[i][1]++;
      }
    }
    // delay__ms(50);
  }
}

void updateDisplayTask() {

  while (1) {

    // drawSpaceship(ship_position_x, ship_position_y, 4);

    if (LPC_GPIO0->PIN & (1 << 29) && (ship_position_x < 24)) {
      spaceship_down = 1;
      ship_position_x++;
    }

    if (LPC_GPIO0->PIN & (1 << 30) && (ship_position_x > 0)) {
      spaceship_down = 1;
      ship_position_x--;
    }

    if (LPC_GPIO0->PIN & (1 << 29) && LPC_GPIO0->PIN & (1 << 30)) {
      while ((gpio_get(0, 29)) && (gpio_get(0, 30)))
        ;

      firing_position();
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
    }
    clearDisplay();
    drawSpaceship(ship_position_x, ship_position_y, 4);

    for (int i = 0; i < bullet_count; i++) {
      if (bulletarray[i][1] < 64) {
        updatePixel(bulletarray[i][0], bulletarray[i][1], 7);
        updatePixel(bulletarray[i][0], ++bulletarray[i][1], 7);
        bulletarray[i][1]++;
      }
    }
    delay__ms(50);
  }
}

void enemyTask1() {
  while (1) {
    int x, enemy_color;
    x = 0 + (rand() % 13);
    enemy_color = (1 + rand() % 6);

    for (int i = 59; i > 0; i--) {
      drawEnemy(x, i, enemy_color);
      delay__ms(50);
    }
  }
}

void enemyTask2() {
  while (1) {
    int x, enemy_color;
    x = 13 + (rand() % 14);
    enemy_color = (1 + rand() % 6);
    for (int i = 59 + (rand() % 10); i > 0; i--) {
      drawEnemy(x, i, enemy_color);
      delay__ms(50);
    }
  }
}

void shoot_isr() {
  shootbool = 1;
  // xSemaphoreGiveFromISR(shoot, NULL);
  LPC_GPIOINT->IO0IntClr |= (1 << 30);
  // portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

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
  puts("Starting RTOS");

  xTaskCreate(updateDisplayTask, "updateDisplayTask", 2048, NULL, 1, NULL);
  xTaskCreate(refreshDisplayTask, "refreshDisplayTask", 2048, NULL, 3, NULL);
  xTaskCreate(enemyTask1, "enemyTask1", 2048, NULL, 2, NULL);
  xTaskCreate(enemyTask2, "enemyTask2", 2048, NULL, 2, NULL);
  // xTaskCreate(fireTask, "fireTask", 2048, NULL, 1, NULL);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}