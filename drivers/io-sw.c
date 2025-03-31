/**
 * @file io-sw.c
 * @author Nachiket Kerai (kerai@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2023-09-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "io-sw.h"
#include "main.h"

/*****************************************************************************/
/* Public Global Variables */
/*****************************************************************************/

/*****************************************************************************/
/* Static Function Declarations */
/*****************************************************************************/
static void Handler_Timer_Buttons(void *handler_arg, cyhal_timer_event_t event);
static void push_buttons_timer_init(void);

/*****************************************************************************/
/* ICE 02 START */
/*****************************************************************************/
void push_buttons_init(bool enable_timer_irq)
{
    /* ADD CODE */
    /* Initialize the IO Pins connected to the push buttons as inputs */

    // Initialize SW2
    cy_rslt_t rslt;
    rslt = cyhal_gpio_init(PIN_SW1, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);
   

    // Initialize SW2
    cy_rslt_t rslt2;
    rslt2 = cyhal_gpio_init(PIN_SW2, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);
    

    // Initialize SW3
    cy_rslt_t rslt3;
    rslt3 = cyhal_gpio_init(PIN_SW3, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);
    

    /* ADD CODE */
    if(enable_timer_irq)
    {
        push_buttons_timer_init();
    }
} 
/*****************************************************************************/
/* ICE 02 END */
/*****************************************************************************/

/*****************************************************************************/
/* ICE 06 START */
/*****************************************************************************/

/*
 * Handler used to debounce the push buttons
*/
static void Handler_Timer_Buttons(void *handler_arg, cyhal_timer_event_t event)
{
    /* Static Vars */
    static uint16_t countSW1 = 0;
    static uint16_t countSW2 = 0;
    static uint16_t countSW3 = 0;
   
    
    uint32_t reg_val = REG_PUSH_BUTTON_IN;

    /* Many mechanical buttons will bounce when they are pressed.
    *  The signal may oscillate between 0 and 1 for several milliseconds.
    *
    * We will debounce a button by sampling the button and detecting the first
    * 50mS interval of time when the button is low.  When that period of time is
    * detected, we will alert the main application using ECE353_Events.
    */

    
    if ((reg_val & SW1_MASK) == 0x00){
        countSW1++;
        if (countSW1 == 5){ 
            ECE353_Events.sw1 = 1;
        }
    } else {
        countSW1 = 0;
    }

    if ((reg_val & SW2_MASK) == 0x00){
        countSW2++;
        if (countSW2 == 5){
            ECE353_Events.sw2 = 1;
        }
    } else {
        countSW2 = 0;
    }

    if ((reg_val & SW3_MASK) == 0x00){
        countSW3++;
        if (countSW3 == 5){
            ECE353_Events.sw3 = 1;
        }
    } else {
        countSW3 = 0;
    }
    
   /* ADD CODE */

}

/* ADD CODE */
/* Allocate static variables for a cyhal_timer_t and cyhal_timer_cfg_t that will be used
 * to configure the button timer*/

static cyhal_timer_t IC07_timer_obj;
static cyhal_timer_cfg_t ICE07_timer_cfg;

static void push_buttons_timer_init(void)
{
    /*ADD CODE*/
    /*
     * Use timer_init() to configure a timer that generates an interrupt every 10mS
     * This timer will be used to debounce the push buttons  on the ECE353 Dev Board
     */
    timer_init(&IC07_timer_obj, &ICE07_timer_cfg, 1000000, Handler_Timer_Buttons);

    cyhal_timer_start(&IC07_timer_obj);

}
/*****************************************************************************/
/* ICE 06 END */
/*****************************************************************************/