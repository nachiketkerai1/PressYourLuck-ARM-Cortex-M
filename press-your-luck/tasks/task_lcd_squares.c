/**
 * @file task_lcd_squares.c
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2024-07-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "task_lcd_squares.h"

#if defined(HW05) || defined(HW06)
#include "main.h"
#include "press-your-luck/pyl.h"
#include "press-your-luck/tasks/task_console.h"

/* Event Groups */
extern EventGroupHandle_t eg_UI;

/* Semaphores */
extern SemaphoreHandle_t s_LCD;

QueueHandle_t q_LCD_SQUARES;

/******************************************************************************
 * FreeRTOS Related Functions
 ******************************************************************************/

/**
 * @brief
 * @param param
 */
void task_lcd_squares(void *param)
{
    /* Suppress warning for unused parameter */
    (void)param;

    image_t image; // image/square to display

    /* Repeatedly running part of the task */
    for (;;)
    {
        if (xQueueReceive(q_LCD_SQUARES, &image, portMAX_DELAY) == pdPASS){ // Receive the square/image data 
            if (xSemaphoreTake(s_LCD, portMAX_DELAY) == pdPASS){
                if (image.reset){
                    pyl_draw_square(image.square_id); // Draw the square
                } else {
                    pyl_draw_image(&image); // Draw the image
                }
                xSemaphoreGive(s_LCD);
            }
        }
    }
}

void task_lcd_squares_init(void)
{

    /* Initialize the LCD hardware */
    ece353_enable_lcd();

    /* Create LCD Queues */
    q_LCD_SQUARES = xQueueCreate(14, sizeof(image_t));

    xTaskCreate(
        task_lcd_squares,
        "Task LCD Squares",
        10 * configMINIMAL_STACK_SIZE,
        NULL,
        ECE353_TASK_PRIORITY_UI_OUTPUT,
        NULL);
}

#endif
