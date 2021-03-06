#include "FreeRTOS.h"
#include "alien_uart.h"
#include "board_io.h"
#include "common_macros.h"
#include "gpio.h"
#include "gpio_driver.h"
#include "led_matrix.h"
#include "portmacro.h"
#include "semphr.h"
#include "sj2_cli.h"
#include "startup_sequence.h"
#include "task.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_DELAY 0xFFFF
#define RECEIVER
#define EXPIRED 64

SemaphoreHandle_t shoot;
bool bullet_collision_1, bullet_collision_2, spacecraft_collision_1, spacecraft_collision_2;
extern uint8_t firing_position_x, firing_position_y, ship_position_x, ship_position_y, enemy1_firing_position_x,
    enemy1_firing_position_y;
extern joystick joystick_control_signal;

uint8_t index1 = 0;
uint8_t index2 = 0;
uint8_t bullet_count = 0;
uint8_t bulletarray[100][2];
uint8_t enemy1_bulletarray[50][2];
uint8_t enemy2_bulletarray[50][2];
uint8_t enemy1_index1 = 0;
uint8_t enemy1_index2 = 0;
uint8_t enemy1_bullet_count = 0;
extern int x1, x2, x3, x4, y1, y2, enemy_color_1, enemy_color_2;

uint16_t kill_count, health;

bool shootbool = false;

void checkCollision(void) {

  // bullet collision
  for (int i = 0; i < bullet_count; i++) {

    if (bulletarray[i][1] != EXPIRED) {

      if ((matrixbuff[bulletarray[i][0]][bulletarray[i][1] + 1]) != 0) {
        // bullet_collision = true;
        bulletarray[i][1] = EXPIRED;
      } else {
        bulletarray[i][1]++;
        if (bulletarray[i][1] > 64)
          bulletarray[i][1] = EXPIRED;
      }
    }
  }

  // spaceship collision

  if ((matrixbuff[x2][y2 - 1] == 7) || (matrixbuff[x2 + 1][y2 - 2] == 7) || (matrixbuff[x2 + 2][y2 - 1] == 7)) {
    bullet_collision_2 = true;
    drawEnemy(x2, y2, 0);
  }
  if ((matrixbuff[x2][y2 - 1] == 4) || (matrixbuff[x2 + 1][y2 - 2] == 4) || (matrixbuff[x2 + 2][y2 - 1] == 4) ||
      (matrixbuff[x2][y2] == 4) || (matrixbuff[x2 + 2][y2] == 4) || (matrixbuff[x2][y2 + 1] == 4) ||
      (matrixbuff[x2 + 2][y2 + 1] == 4) || (matrixbuff[x2][y2 + 2] == 4) || (matrixbuff[x2 + 2][y2 + 2] == 4) ||
      (matrixbuff[x2][y2 + 3] == 4) || (matrixbuff[x2 + 2][y2 + 3] == 4))
  //|| matrixbuff[x2][y2 - 1] == 5)
  {
    spacecraft_collision_2 = true;
    puts("\nenemy2 collision");
    drawEnemy(x2, y2, 0);
  }

  if ((matrixbuff[x1][y1 - 1] == 7) || (matrixbuff[x1 + 1][y1 - 2] == 7) || (matrixbuff[x1 + 2][y1 - 1] == 7)) {
    bullet_collision_1 = true;

    // burst_animation(x1, y1, 7);
    // delay__ms(30);
    // burst_animation(x1, y1, 0);
    drawEnemy(x1, y1, 0);
  }
  if ((matrixbuff[x1][y1 - 1] == 4) || (matrixbuff[x1 + 1][y1 - 2] == 4) || (matrixbuff[x1 + 2][y1 - 1] == 4) ||
      (matrixbuff[x1][y1] == 4) || (matrixbuff[x1 + 2][y1] == 4) || (matrixbuff[x1][y1 + 1] == 4) ||
      (matrixbuff[x1 + 2][y1 + 1] == 4) || (matrixbuff[x1][y1 + 2] == 4) || (matrixbuff[x1 + 2][y1 + 2] == 4) ||
      (matrixbuff[x1][y1 + 3] == 4) || (matrixbuff[x1 + 2][y1 + 3] == 4))
  //|| matrixbuff[x1][y1 - 1] == 5)
  {
    spacecraft_collision_1 = true;
    drawEnemy(x1, y1, 0);
  }

  // missile collision
}

void refreshDisplayTask() {

  while (1) {
    drawEnemy(x1, y1, enemy_color_1);
    drawEnemy(x2, y2, enemy_color_2);
    // if (health >= 50) {
    // drawSpaceship(ship_position_x, ship_position_y, 5);
    //} else {
    drawSpaceship(ship_position_x, ship_position_y, 4);
    //}
    updateDisplay();
    checkCollision();

#if 0
    if (bullet_collision_1 == true || spacecraft_collision_1 == true) {
      drawEnemy(x1, y1, 0);
    }
    if (bullet_collision_2 == true || spacecraft_collision_2 == true) {
      drawEnemy(x2, y2, 0);
    }
#endif
    clearDisplay();
    vTaskDelay(3);
  }
}

void fireTask() {

  while (1) {

    if (shootbool) {
      // delay__ms(30);
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

void enemy1_fireTask() {

  while (1) {

    enemy1_firing_position();

    enemy1_bulletarray[enemy1_index1][enemy1_index2++] = enemy1_firing_position_x;
    enemy1_bulletarray[enemy1_index1++][enemy1_index2] = enemy1_firing_position_y;
    enemy1_bullet_count++;

    if (enemy1_index1 == 50) {
      enemy1_index1 = 0;
    }
    if (enemy1_index1 == 1) {
      enemy1_index1 = 0;
    }
    if (enemy1_bullet_count == 50) {
      enemy1_bullet_count = 0;
    }

    for (x4 = 0; x4 < enemy1_bullet_count; x4++) {

      if (enemy1_bulletarray[x3][1] < 64) {

        updatePixel(enemy1_bulletarray[x3][0], enemy1_bulletarray[x3][1], 7);
        updatePixel(enemy1_bulletarray[x3][0], ++enemy1_bulletarray[x3][1], 7);

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

    //  bullet
    if (gpio_get(0, 29)) {
      delay__ms(30);
      if (gpio_get(0, 29))
        shootbool = true;
    }

    // missile

    vTaskDelay(40);
  }
}

void enemyTask1() {

  while (1) {

    x1 = 2 + (rand() % 11); // 32/2 = 16-3 = 13
    enemy_color_1 = (1 + rand() % 6);

    for (y1 = 59; y1 > 0; y1--) {
      delay__ms(50);
      if (bullet_collision_1 == true || spacecraft_collision_1 == true) {
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
    // delay__ms(30);
    for (y2 = 59 + (rand() % 10); y1 > 0; y2--) {
      delay__ms(50);
      if (bullet_collision_2 == true || spacecraft_collision_2 == true) {
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

  gpio_set(2, 2, 0);

  gpio_set(2, 2, 1);
  delay__ms(10);
  gpio_set(2, 2, 0);

  puts("Starting RTOS");
  // (void)startup_screen();
  srand(time(0));
  xTaskCreate(refreshDisplayTask, "refreshDisplayTask", 2048, NULL, 3, NULL);
  xTaskCreate(enemyTask1, "enemyTask1", 2048, NULL, 1, NULL);
  xTaskCreate(enemyTask2, "enemyTask2", 2048, NULL, 1, NULL);
  xTaskCreate(updateSpaceshipTask, "updateSpaceshipTask", 2048, NULL, 1, NULL);
  xTaskCreate(fireTask, "fireTask", 2048, NULL, 1, NULL);
  // xTaskCreate(enemy1_fireTask, "fireTask", 2048, NULL, 1, NULL);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}