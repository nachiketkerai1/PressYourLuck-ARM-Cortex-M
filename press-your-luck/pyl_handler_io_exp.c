/**
 * @file pyl_handler_tmr_050_ms.c
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief
 * @version 0.1
 * @date 2024-08-13
 *
 * @copyright Copyright (c) 2024
 *
 */
 #include "cyhal_gpio.h"
#include "ece353_events.h"
#include "io-expander.h"
#include "pyl.h"
 #include "pyl_handler_io_exp.h"


 #if defined (HW04) || defined (HW05)


/**
 * Allocate a global variable of type cyhal_gpio_callback_data_t used to
 * initialize interrupts on a gpio pin
 */


//cyhal_gpio_callback_data_t interruptVar;
//extern EventGroupHandle_t eg_UI;


/**
 * @brief
 * This function assumes that i2c_init() has already been executed.
 *
 * Initialize the IO expander so bit 7 is an input and bits 6-0 are outputs.
 *
 * Initialize the PSoC6 pin connected to the IO expanders interrupt output pin
 * to generate an interrupt when push button (U602) is pressed  
 */
/*
void io_exp_initialize(void)
{
    // Initialize the IO expander
    uint8_t ioVal = 0x80; // Fix
    io_expander_set_configuration(ioVal);
    io_expander_set_output_port(0x00);

    interruptVar.callback = handler_io_exp;
    interruptVar.callback_arg = NULL;

    uint8_t button_state = io_expander_get_input_port();


    // Initialize the PSoC6 pin
    cyhal_gpio_init(IO_PIN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false); // I dont know what pin to intialize it with
    cyhal_gpio_register_callback(IO_PIN, &interruptVar);
    cyhal_gpio_enable_event(IO_PIN, CYHAL_GPIO_IRQ_FALL, 3, true);
} */


/**
 * @brief
 * Handler used to inform non-isr code that a the IO expander push button has been pressed
 * @param callback_arg
 * @param event
 */
 /*
 void handler_io_exp(void *callback_arg, cyhal_gpio_event_t event)
 {
    ECE353_Events.io_button = 1;
        
 }*/


#endif
