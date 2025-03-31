/**
 * @file task_game_control.c
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2024-08-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "task_game_control.h"
#include "portmacro.h"
#include "task_io_expander.h"
#include "task_lcd_squares.h"
#include "task_lcd_game_info.h"

#if defined(HW05)

#define DEFAULT_RANDOM_DELAY 1500
#define DEFAULT_ACTIVE_DELAY 400

#include "main.h"
#include "press-your-luck/pyl.h"
#include "press-your-luck/tasks/task_console.h"
#include "press-your-luck/tasks/task_eeprom.h"
#include "press-your-luck/tasks/ece353_tasks.h"

extern cyhal_trng_t trng_obj;
extern uint8_t playerState = 1; // 0 = inactive player, 1 = active player
bool timerUpdate = false;

char STUDENTS[] = "Nachiket Kerai";
char TEAM[] = "Team46";


/* Event Groups */
extern EventGroupHandle_t eg_UI;

/* Semaphores */
extern SemaphoreHandle_t s_LCD;

/* Queues */

TaskHandle_t Task_Handle_Game_Control;
TimerHandle_t Timer_Handle_Randomize;
TimerHandle_t Timer_Handle_Active_Square;
QueueHandle_t q_Control_Rx_Data;
QueueHandle_t q_Control_EEPROM_Rx;

void timer_randomize_callback(TimerHandle_t xTimer)
{
  /* Set the  in the event group */
  xEventGroupSetBits(eg_UI, EVENT_TMR_RAND_SQUARES);
}

void timer_active_callback(TimerHandle_t xTimer)
{
  /* Set the  in the event group */
  xEventGroupSetBits(eg_UI, EVENT_TMR_RAND_ACTIVE);
}

/******************************************************************************/
/******************************************************************************/

void task_game_control(void *param)
{
  EventBits_t active_events;
  game_info_msg_t local_stats= {.cmd = CMD_UPDATE_LOCAL_DATA, .score = 0, .spins = 8, .passed = 0};
  game_info_msg_t remote_stats= {.cmd = CMD_UPDATE_REMOTE_DATA, .score = 0, .spins = 0, .passed = 0};
  uint8_t num_whammies = 0;
  uint16_t hi_score_write = 0;
  uint16_t hi_score_read = 0;

  static int8_t prev_active_index = -1;
  uint8_t new_active_index;

  image_t *pyl_images = pvPortMalloc(14*sizeof(image_t));

  /* Suppress warning for unused parameter */
  (void)param;

    /* Send the Clear Screen Escape Sequence*/
    task_print("\x1b[2J\x1b[;H");

    task_print("**************************************************\n\r");
    task_print("* %s\n\r", APP_DESCRIPTION);
    task_print("* Date: %s\n\r", __DATE__);
    task_print("* Time: %s\n\r", __TIME__);
    task_print("* Team:%s\n\r", TEAM);
    task_print("* Students:%s\n\r", STUDENTS);
    task_print("**************************************************\n\r");


    {
      eeprom_msg_t curr_read_msg = {.operation = EEPROM_READ, .score = 0, .return_queue = q_Control_EEPROM_Rx};
      xQueueSend(q_EEPROM, &curr_read_msg, portMAX_DELAY);

      eeprom_msg_t response;
      xQueueReceive(q_Control_EEPROM_Rx, &response, portMAX_DELAY);
      hi_score_read = response.score;
      task_print("Current High Score: %u\n\r", hi_score_read);
    }

    /* Start the random image timer */
    xTimerStart(Timer_Handle_Randomize, portMAX_DELAY);

    /* Start the Random Square Timer*/
    xTimerStart(Timer_Handle_Active_Square, portMAX_DELAY);

  /* Repeatedly running part of the task */
  for (;;)
  {

    /* Clear events that may have occurred when not in the Active state */
    xEventGroupClearBits(eg_UI, 0xFFFFFF);

    xEventGroupSetBits(eg_UI, EVENT_START_GAME);

    

    /* ADD CODE */
    /* Draw empty squares */
    for (int i = 0; i < 14; i++) {
        pyl_images[i].square_id = i; // set square_id
        pyl_images[i].reset = true;
        xQueueSend(q_LCD_SQUARES, &pyl_images[i], portMAX_DELAY);
    } 

    while (1)
    {
      /* Wait for the user to press SW1 or move move attempt to move the board */
      active_events = xEventGroupWaitBits(
          eg_UI,
          EVENT_UI_SW1 | 
          EVENT_UI_SW2 | 
          EVENT_UI_SW3 | 
          EVENT_TMR_RAND_SQUARES | 
          EVENT_TMR_RAND_ACTIVE |
          EVENT_IPC_RX_DATA |
          EVENT_START_GAME | EVENT_UI_IO_BUTTON , 
          true,  /* Clear Events on Return*/
          false, /* Any Event Wakes up task*/
          portMAX_DELAY);

        eeprom_msg_t curr_read = {
          .operation = EEPROM_READ,
          .return_queue = q_Control_EEPROM_Rx
        };

        if (xQueueSend(q_EEPROM, &curr_read, portMAX_DELAY) != pdPASS) {
          task_print_info("Failed to send high score read message to EEPROM task");
        }

        // Receive the EEPROM response 
        eeprom_msg_t response;
        if (xQueueReceive(q_Control_EEPROM_Rx, &response, portMAX_DELAY) != pdPASS) {
          task_print_info("Failed to receive high score from EEPROM task");
        }

        hi_score_read = response.score;

      // Check if there is a game over
      if (local_stats.spins ==  0){
        game_info_msg_t msg = {.cmd = CMD_GAME_OVER, .passed = local_stats.passed, .score = local_stats.score, .spins = local_stats.spins};
        xTimerStop(Timer_Handle_Randomize, 0);
        xTimerStop(Timer_Handle_Active_Square, 0);

                        // Update high score if needed
        if (local_stats.score > hi_score_read) {
            hi_score_write = local_stats.score;
            eeprom_msg_t write_msg = {.operation = EEPROM_WRITE, .score = hi_score_write, .return_queue = NULL};
              xQueueSend(q_EEPROM, &write_msg, portMAX_DELAY);
              task_print_info("New high score saved to EEPROM: %d", hi_score_write);
        } else {
          task_print_info("Game Over! High score remains unchanged.");
        }
        xQueueSend(q_LCD_Info, &msg, portMAX_DELAY);
      }
     
      // Randomize the board and randomly select an active square
      if ((active_events & EVENT_TMR_RAND_ACTIVE) == EVENT_TMR_RAND_ACTIVE){
        //static uint8_t prev_active_index = -1;
        new_active_index = cyhal_trng_generate(&trng_obj) % 14;

        // Reset previous active image if any
        if (prev_active_index != -1){
          pyl_images[prev_active_index].invert_colors = false;
          pyl_images[prev_active_index].square_id = prev_active_index;
          xQueueSend(q_LCD_SQUARES, &pyl_images[prev_active_index], portMAX_DELAY);
        }

        // Set the new active image
        pyl_images[new_active_index].invert_colors = true;
        pyl_images[new_active_index].square_id = new_active_index;
        xQueueSend(q_LCD_SQUARES, &pyl_images[new_active_index], portMAX_DELAY);

        prev_active_index = new_active_index;

      } else if ((active_events & EVENT_TMR_RAND_SQUARES) == EVENT_TMR_RAND_SQUARES){
        pyl_images_randomize(pyl_images); // Randomize the game board
        // Print the newly randomized images 
        for (uint8_t i = 0; i < 14; i++) {
          pyl_images[i].square_id = i;
          pyl_images[i].reset = false;
          if (xQueueSend(q_LCD_SQUARES, &pyl_images[i], portMAX_DELAY) != pdPASS){
            task_print_info("Failed to send image %d to LCD queue", i);
          }
        }
      }

      // Display the local info
      xQueueSend(q_LCD_Info, &local_stats, portMAX_DELAY);
      

      /* Check to see which event is active */
      if( (active_events & EVENT_UI_SW1) == EVENT_UI_SW1)
      {
        task_print_info("SW1 Pressed");

        if (prev_active_index != -1){
        // Blink the active square 3 times
          for (int i = 0; i < 3; i++){
            // Turn off the active image
            pyl_images[prev_active_index].invert_colors = false;
            xQueueSend(q_LCD_SQUARES, &pyl_images[prev_active_index], portMAX_DELAY);
            vTaskDelay(pdMS_TO_TICKS(750));  // Delay for 750 ms

            // Turn on the active image
            pyl_images[prev_active_index].invert_colors = true;
            xQueueSend(q_LCD_SQUARES, &pyl_images[prev_active_index], portMAX_DELAY);
            vTaskDelay(pdMS_TO_TICKS(750));  // Delay for 750 ms
          }

          // Ensure the active image is on after blinking
          pyl_images[prev_active_index].invert_colors = true;
          xQueueSend(q_LCD_SQUARES, &pyl_images[prev_active_index], portMAX_DELAY);
        }

        if (pyl_images[prev_active_index].image_type == IMG_TYPE_WHAMMY)
            {
                local_stats.score = 0;
                local_stats.spins--;
                num_whammies++;
                if (xQueueSend(q_IO_Exp, &num_whammies, portMAX_DELAY) != pdPASS){
                  task_print_info("Failed to Send num_whammies to IO Expander task");
                }
                // Logic to ensure that the game stops when 4 whammies hit
                if (num_whammies == 4){
                  local_stats.spins = 0;
                }
            }
            else if (pyl_images[prev_active_index].image_type == IMG_TYPE_1000_PLUS_SPIN)
            {
                local_stats.score = local_stats.score + 1000;
            }
            else if (pyl_images[prev_active_index].image_type == IMG_TYPE_2500_PLUS_SPIN)
            {
                local_stats.score = local_stats.score + 2500;
            }
            else if (pyl_images[prev_active_index].image_type == IMG_TYPE_5000_PLUS_SPIN)
            {
                local_stats.score = local_stats.score + 5000;
            }
            else if (pyl_images[prev_active_index].image_type == IMG_TYPE_0100)
            {
                local_stats.score = local_stats.score + 100;
                local_stats.spins--;
            }
            else if (pyl_images[prev_active_index].image_type == IMG_TYPE_0250)
            {
                local_stats.score = local_stats.score + 250;
                local_stats.spins--;
            }
            else if (pyl_images[prev_active_index].image_type == IMG_TYPE_0500)
            {
                local_stats.score = local_stats.score + 500;
                local_stats.spins--;
            }
            else if (pyl_images[prev_active_index].image_type == IMG_TYPE_1000)
            {
                local_stats.score = local_stats.score + 1000;
                local_stats.spins--;
            }
            else if (pyl_images[prev_active_index].image_type == IMG_TYPE_2500)
            {
                local_stats.score = local_stats.score + 2500;
                local_stats.spins--;
            }
            else if (pyl_images[prev_active_index].image_type == IMG_TYPE_5000)
            {
                local_stats.score = local_stats.score + 5000;
                local_stats.spins--;
            }
       
      }
      else if( (active_events & EVENT_UI_SW2) == EVENT_UI_SW2)
      {
        task_print_info("SW2 Pressed");

      }
      else if( (active_events & EVENT_UI_SW3) == EVENT_UI_SW3)
      {
        task_print_info("SW3 Pressed");

        task_print_info("SW3 Pressed: Starting a New Game");

        local_stats.score = 0;
        local_stats.spins = 8;
        local_stats.passed = 0;
        num_whammies = 0;
        xQueueSend(q_IO_Exp, &num_whammies, portMAX_DELAY);

        // Reset the game over
        lcd_draw_image(IMAGE_COORD_X_GAME_OVER, IMAGE_COORD_Y_GAME_OVER, IMG_BITMAP_WIDTH_GAME_OVER, IMG_BITMAP_HEIGHT_GAME_OVER, IMG_BITMAP_GAME_OVER, LCD_COLOR_BLACK, LCD_COLOR_BLACK);

        // Read and display High Score again when starting a new game
        {
          eeprom_msg_t curr_read_msg = {.operation = EEPROM_READ, .score = 0, .return_queue = q_Control_EEPROM_Rx};
          xQueueSend(q_EEPROM, &curr_read_msg, portMAX_DELAY);

          eeprom_msg_t response;
          xQueueReceive(q_Control_EEPROM_Rx, &response, portMAX_DELAY);
          hi_score_read = response.score;
         task_print("Current High Score: %u\n\r", hi_score_read);
        }

        xTimerStart(Timer_Handle_Randomize, portMAX_DELAY);
        xTimerStart(Timer_Handle_Active_Square, portMAX_DELAY);

        for (int i = 0; i < 14; i++) {
          pyl_images[i].square_id = i;
          pyl_images[i].reset = true;
          pyl_images[i].invert_colors = false;
          xQueueSend(q_LCD_SQUARES, &pyl_images[i], portMAX_DELAY);
        }

        xQueueSend(q_LCD_Info, &local_stats, portMAX_DELAY);

      }
      else if( (active_events & EVENT_IPC_RX_DATA) == EVENT_IPC_RX_DATA)
      {
        task_print_info("Revived Remote Packet");
      } else if ((active_events & EVENT_UI_IO_BUTTON) == EVENT_UI_IO_BUTTON){

        eeprom_msg_t reset_msg = {.operation = EEPROM_WRITE, .score = 0, .return_queue = NULL};
        xQueueSend(q_EEPROM, &reset_msg, portMAX_DELAY);
      }
    }
  }
}

void task_game_control_init(void)
{
  q_Control_Rx_Data = xQueueCreate(1, sizeof(game_info_msg_t));
  q_Control_EEPROM_Rx = xQueueCreate(1, sizeof(eeprom_msg_t));

  /* Create a timer for randomizing the image */
  Timer_Handle_Randomize = xTimerCreate(
      "Randomize",
      DEFAULT_RANDOM_DELAY,
      true, (void *)0,
      timer_randomize_callback);

  /* Create a timer for randomizing the position of
     the active square */
  Timer_Handle_Active_Square = xTimerCreate(
      "Active Square",
      DEFAULT_ACTIVE_DELAY,
      true,
      (void *)0,
      timer_active_callback);

  xTaskCreate(
      task_game_control,
      "Task Game Control",
      10 * configMINIMAL_STACK_SIZE,
      NULL,
      ECE353_TASK_PRIORITY_FSM,
      &Task_Handle_Game_Control);
}

#endif
