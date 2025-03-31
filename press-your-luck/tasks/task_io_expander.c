/**
 * @file task_io_expander.c
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2024-08-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "task_io_expander.h"
#include "ece353_tasks.h"

#if defined(HW05) || defined (HW06)
#include "main.h"
#include "press-your-luck/pyl.h"
#include "press-your-luck/tasks/task_console.h"

/* Event Groups */
extern EventGroupHandle_t eg_UI;
cyhal_gpio_callback_data_t interruptVar;

QueueHandle_t q_IO_Exp;


/**
 * @brief
 * Handler used to inform non-isr code that a the IO expander push button has been pressed
 * @param callback_arg
 * @param event
 */
 void handler_io_exp(void *callback_arg, cyhal_gpio_event_t event)
 {
    //ECE353_Events.io_button = 1;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xEventGroupSetBitsFromISR(eg_UI, EVENT_UI_IO_BUTTON, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        
 }

/**
 * @brief 
 * Displays the number of whammies that have been selected
 * @param param 
 * Not Used
 */
void task_io_expander(void *param)
{
    /* Suppress warning for unused parameter */
    (void)param;

    io_expander_set_configuration(0x80); // Set bit 7 as input, bits 6-0 as outputs
    io_expander_set_output_port(0x00);   // Turn OFF all LEDs

    uint8_t numWhammies;
    uint8_t outputValue;

    /* Repeatedly running part of the task */
    for (;;)
    {
        /* ADD CODE */
        if (xQueueReceive(q_IO_Exp, &numWhammies, portMAX_DELAY) == pdPASS){

            switch (numWhammies){
                case 4:
                    io_expander_set_output_port(~0xF0);
                    break;
                case 3:
                    io_expander_set_output_port(~0xF1);
                    break;
                case 2:
                    io_expander_set_output_port(~0xF3);
                    break;
                case 1:
                    io_expander_set_output_port(~0xF7);
                    break;
                case 0:
                    io_expander_set_output_port(~0xFF);
                    break;
                default:

                    break;
            }
        } 

    }
}

void task_io_expander_init(void)
{
    i2c_init();

    q_IO_Exp = xQueueCreate(1, sizeof(uint8_t));

    // Initialize the IO expander
    uint8_t ioVal = 0x80; // Fix
    io_expander_set_configuration(ioVal);
    io_expander_set_output_port(0x00);

    interruptVar.callback = handler_io_exp;
    interruptVar.callback_arg = NULL;

    uint8_t button_state = io_expander_get_input_port();


    // Initialize the PSoC6 pin
    cyhal_gpio_init(P11_2, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false); // I dont know what pin to intialize it with
    cyhal_gpio_register_callback(P11_2, &interruptVar);
    cyhal_gpio_enable_event(P11_2, CYHAL_GPIO_IRQ_FALL, 3, true);

    xTaskCreate(
        task_io_expander,
        "Task IO Exp",
        configMINIMAL_STACK_SIZE,
        NULL,
        ECE353_TASK_PRIORITY_UI_INPUT,
        NULL);
}

#endif
