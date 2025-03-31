/**
 * @file task_sw.c
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2024-08-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "task_sw.h"

#if defined(HW05) || defined(HW06)
#include "main.h"
#include "press-your-luck/pyl.h"
#include "press-your-luck/tasks/task_console.h"

/* Event Groups */
extern EventGroupHandle_t eg_UI;

/**
 * @brief
 * Function used to detect when SW1, SW2, and SW3 are been pressed.  
 * The task should detect the initial button press by debouncing 
 * the push button for 50mS.  Pressing the button for longer than 50mS 
 * should only result in a single button event.
 * 
 * @param param
 *  Unused
 */
void task_switches(void *param)
{

  uint32_t reg_val;
  bool sw1_curr = false;
  bool sw1_prev = false;
  bool sw2_curr = false;
  bool sw2_prev = false;
  bool sw3_curr = false;
  bool sw3_prev = false;

  bool io_curr = false;
  bool io_prev = false;

  while (1)
  {
    vTaskDelay(50);
    /* ADD CODE */
    reg_val = PORT_BUTTONS->IN;
    uint8_t button_state = io_expander_get_input_port();


    if ((reg_val & SW1_MASK) == 0){
      sw1_curr = true;
    } else {
      sw1_curr = false;
    }

    if ((reg_val & SW2_MASK) == 0){
      sw2_curr = true;
    } else {
      sw2_curr = false;
    }

    if ((reg_val & SW3_MASK) == 0){
      sw3_curr = true;
    } else {
      sw3_curr = false;
    }

    if (sw1_curr && !sw1_prev){
      //task_print_info("SW1 Falling Edge Detected");

      xEventGroupSetBits(eg_UI, EVENT_UI_SW1);
    }

    if (sw2_curr && !sw2_prev){
      //task_print_info("SW2 Falling Edge Detected");

      xEventGroupSetBits(eg_UI, EVENT_UI_SW2);
    }

    if (sw3_curr && !sw3_prev){

      //task_print_info("SW3 Falling Edge Detected");

      xEventGroupSetBits(eg_UI, EVENT_UI_SW3);
    }

    /*
    if (io_curr && !io_prev){
      xEventGroupSetBits(eg_UI, EVENT_UI_IO_BUTTON);
    }*/

    sw1_prev = sw1_curr;
    sw2_prev = sw2_curr;
    sw3_prev = sw3_curr;
    io_prev = io_curr;
  }
}

/**
 * @brief
 * Initializes User Push Buttons and creates the corresponding FreeRTOS tasks for SW1 and SW2
 */
void task_sw_init(void)
{
  push_buttons_init(false);

  // Register the Task with FreeRTOS
  xTaskCreate(
      task_switches,
      "Task Switches",
      configMINIMAL_STACK_SIZE,
      NULL,
      ECE353_TASK_PRIORITY_UI_INPUT,
      NULL);
}

#endif
