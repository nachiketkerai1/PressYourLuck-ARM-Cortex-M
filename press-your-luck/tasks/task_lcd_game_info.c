/**
 * @file task_lcd_game_info.c
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief
 * @version 0.1
 * @date 2024-07-31
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "task_lcd_game_info.h"

#if defined(HW05) || defined(HW06)

#include "main.h"
#include "press-your-luck/pyl.h"
#include "press-your-luck/tasks/task_console.h"

/* Event Groups */
extern EventGroupHandle_t eg_UI;

/* Semaphores */
extern SemaphoreHandle_t s_LCD;

QueueHandle_t q_LCD_Info;

/******************************************************************************
 * FreeRTOS Related Functions
 ******************************************************************************/

/**
 * @brief
 * @param param
 */
void task_lcd_game_info(void *param)
{
    /* Suppress warning for unused parameter */
    (void)param;

    game_info_msg_t message;

    /* Repeatedly running part of the task */
    for (;;)
    {
        if (xQueueReceive(q_LCD_Info, &message, portMAX_DELAY) == pdPASS){ // Receive the message to display
            if (xSemaphoreTake(s_LCD, portMAX_DELAY) == pdPASS){
                switch (message.cmd) {
                    case CMD_UPDATE_LOCAL_DATA:
                        // Logic to print the local data
                        pyl_print_digits_6(message.score, LOCATION_LOCAL_SCORE_X, LOCATION_LOCAL_SCORE_Y, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
                        pyl_print_digits_2(message.spins, LOCATION_LOCAL_SPINS_X, LOCATION_LOCAL_SPINS_Y, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
                        pyl_print_digits_2(message.passed, LOCATION_LOCAL_PASSED_X, LOCATION_LOCAL_PASSED_Y, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
                        break;
                    /*
                    case CMD_UPDATE_REMOTE_DATA:
                        // Logic to print the remote data
                        pyl_print_digits_6(message.score, LOCATION_REMOTE_SCORE_X, LOCATION_REMOTE_SCORE_Y, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
                        pyl_print_digits_2(message.spins, LOCATION_REMOTE_SPINS_X, LOCATION_REMOTE_SPINS_Y, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
                        pyl_print_digits_2(message.passed, LOCATION_REMOTE_PASSED_X, LOCATION_REMOTE_PASSED_Y, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
                        break;
                    */ 
                    case CMD_GAME_OVER:
                        // Reset all the fields, update the local game info
                        lcd_draw_image(IMAGE_COORD_X_GAME_OVER, IMAGE_COORD_Y_GAME_OVER, IMG_BITMAP_WIDTH_GAME_OVER, IMG_BITMAP_HEIGHT_GAME_OVER, &IMG_BITMAP_GAME_OVER, LCD_COLOR_RED, LCD_COLOR_BLACK);
                        break;
                }
                xSemaphoreGive(s_LCD);
            }
        }
    }
    
}

void task_lcd_game_info_init(void)
{
    q_LCD_Info = xQueueCreate(1,sizeof(game_info_msg_t));

    xTaskCreate(
        task_lcd_game_info,
        "Task LCD Game Info",
        5 * configMINIMAL_STACK_SIZE,
        NULL,
        ECE353_TASK_PRIORITY_UI_OUTPUT,
        NULL);
}

#endif
