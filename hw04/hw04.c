/**
 * @file hw04.c
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief
 * @version 0.1
 * @date 2024-01-08
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "hw04.h"

#if defined(HW04)

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/
char APP_DESCRIPTION[] = "ECE353: HW04 Solutions";
char STUDENTS[] = "Joe Krachey";

/* Random Number Generator Handle*/
cyhal_trng_t trng_obj;

/* Current Position of the Joystick */
joystick_position_t Joystick_Pos = JOYSTICK_POS_CENTER;

/* 50mS Timer Handles*/
cyhal_timer_t Hw_Timer_Obj; 
cyhal_timer_cfg_t Hw_Timer_Cfg;

/**
 * @brief
 * This function will initialize all of the hardware resources for
 * the ICE.
 *
 * This function is implemented in the iceXX.c file for the ICE you are
 * working on.
 */
void peripheral_init(void)
{
    console_init();

    /* Initialize the hardware random number generator*/
    cyhal_trng_init(&trng_obj);

    /* Enable the push buttons*/
    push_buttons_init(true);

    ece353_enable_lcd();

    pwm_buzzer_init();

    joystick_init();

    /* Enable a timer peripheral that will interrupt every 50mS */
    timer_init(
        &Hw_Timer_Obj, 
        &Hw_Timer_Cfg, 
        5000000,
        handler_timer_050_ms);

    cyhal_timer_start(&Hw_Timer_Obj);

    /* Configure the remote UART for 115200, 8N1*/
    remote_uart_init();

    /* Enable Rx interrupts for the remote.  Use the handler defined in pyl_ipc.c */
    remote_uart_enable_interrupts(handler_ipc, true, true);

    /* Enable the I2C interface */
    i2c_init();

    /* Configure the IO Expander */
    io_exp_initialize();

    /* Enable the SPI interface */
    spi_init();

    /* Configure the IO pin used to control the chip select*/
    eeprom_cs_init();

}

/*****************************************************************************/
/* Application Code                                                          */
/*****************************************************************************/
/**
 * @brief
 * This function implements the behavioral requirements for the ICE
 *
 * This function is implemented in the iceXX.c file for the ICE you are
 * working on.
 */
void main_app(void)
{
    image_t *pyl_images = malloc(14 * sizeof(image_t));
    uint8_t active_index = 0;
    game_info_msg_t local_info = {.cmd = CMD_UPDATE_LOCAL_DATA, .score = 0, .spins = 5, .passed = 0};
    game_info_msg_t remote_info = {0};  // Remote game information
    uint8_t numWhammies = 0;
    uint16_t highScore = 0;

    /* Send the Clear Screen Escape Sequence*/
    printf("\x1b[2J\x1b[;H");

    printf("**************************************************\n\r");
    printf("* %s\n\r", APP_DESCRIPTION);
    printf("* Date: %s\n\r", __DATE__);
    printf("* Time: %s\n\r", __TIME__);
    printf("* Name:%s\n\r", NAME);
    printf("**************************************************\n\r");

    uint8_t low_byte = (uint8_t)(highScore & 0x00FF);
    uint8_t high_byte = (uint8_t)((highScore >> 8) & 0x00FF);
           
    //eeprom_write_byte(0x20, low_byte);
    //eeprom_write_byte(0x21, high_byte);


    pyl_images_randomize(pyl_images);
    pyl_images[0].invert_colors = true;
    for (int i = 0; i < 14; i++) {
        pyl_images[i].square_id = i;
        pyl_draw_square(i);
        pyl_draw_image(&pyl_images[i]);
    }
    // Set LEDs initially to 0;
    io_expander_set_output_port(~0xFF);
    printf("Current High Score: %06d\n\r", pyl_eeprom_high_score_read());

    for (;;)
    {

        /* Display content to the board */
        pyl_print_digits_6(local_info.score, LOCATION_LOCAL_SCORE_X, LOCATION_LOCAL_SCORE_Y, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
        pyl_print_digits_2(local_info.spins, LOCATION_LOCAL_SPINS_X, LOCATION_LOCAL_SPINS_Y, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
        pyl_print_digits_2(local_info.passed, LOCATION_LOCAL_PASSED_X, LOCATION_LOCAL_PASSED_Y, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
        
        /* If SW1 is pressed, update the scores and spins */
        if (ECE353_Events.sw1 && local_info.spins != 0)
        {
            // Update score and spins based on active square
            ECE353_Events.sw1 = 0;
            if (pyl_images[active_index].image_type == IMG_TYPE_WHAMMY)
            {
                //local_info.score = 0;
                local_info.spins--;
                numWhammies++;
                /*Display the number of whammies the user has selected*/    
                if (numWhammies == 1){
                    /*Update displayed numWhammies on led*/
                    // Write to output port reg TCA9534_OUTPUT_PORT_ADDR, look at the schematic of the io expander led, pin 3
                    // io_expander_write_reg
                    io_expander_set_output_port(~0xF7);
                } else if (numWhammies == 2){
                    // Write to output with eeprom write to reg,
                    io_expander_set_output_port(~0xF3);
                } else if (numWhammies == 3){
                    io_expander_set_output_port(~0xF1);
                } else if (numWhammies == 4){
                    /*Set users local score and spins to zero*/
                    local_info.score = 0;
                    local_info.spins = 0;
                    io_expander_set_output_port(~0xF0);
                }
            }
            else if (pyl_images[active_index].image_type == IMG_TYPE_1000_PLUS_SPIN)
            {
                local_info.score = local_info.score + 1000;
            }
            else if (pyl_images[active_index].image_type == IMG_TYPE_2500_PLUS_SPIN)
            {
                local_info.score = local_info.score + 2500;
            }
            else if (pyl_images[active_index].image_type == IMG_TYPE_5000_PLUS_SPIN)
            {
                local_info.score = local_info.score + 5000;
            }
            else if (pyl_images[active_index].image_type == IMG_TYPE_0100)
            {
                local_info.score = local_info.score + 100;
                local_info.spins--;
            }
            else if (pyl_images[active_index].image_type == IMG_TYPE_0250)
            {
                local_info.score = local_info.score + 250;
                local_info.spins--;
            }
            else if (pyl_images[active_index].image_type == IMG_TYPE_0500)
            {
                local_info.score = local_info.score + 500;
                local_info.spins--;
            }
            else if (pyl_images[active_index].image_type == IMG_TYPE_1000)
            {
                local_info.score = local_info.score + 1000;
                local_info.spins--;
            }
            else if (pyl_images[active_index].image_type == IMG_TYPE_2500)
            {
                local_info.score = local_info.score + 2500;
                local_info.spins--;
            }
            else if (pyl_images[active_index].image_type == IMG_TYPE_5000)
            {
                local_info.score = local_info.score + 5000;
                local_info.spins--;
            }


            /* Transmit updated local data to remote board */
            /*printf("I am doing this\n");
            fflush(stdout);
            cyhal_system_delay_ms(10);*/
            pyl_ipc_tx(&local_info);

            ECE353_Events.sw1 = 0;

        }


        /* If SW2 is pressed, randomize the 14 squares and display them on the LCD*/
        if (ECE353_Events.sw2 == 1 && local_info.spins != 0)
        {
            ECE353_Events.sw2 = 0;
            active_index = 0;
            pyl_images_randomize(pyl_images);
            pyl_images[active_index].invert_colors = true;
            for (uint8_t i = 0; i < NUM_SQUARES; i++) {
                pyl_images[i].square_id = i;
            }

            for (uint8_t i = 0; i < NUM_SQUARES; i++) {
                pyl_draw_image(&(pyl_images[i]));
            }
        }
        
        /* If joystick is moved from the center, move the active square */
        if (ECE353_Events.joystick == 1 && local_info.spins != 0)
        {
            ECE353_Events.joystick = 0;
            pyl_images[active_index].invert_colors = true;
            pyl_draw_image(&pyl_images[active_index]);  // Draw the new active image
            if (joystick_get_pos() == JOYSTICK_POS_UP)
            {
                pyl_images[active_index].invert_colors = false;
                pyl_draw_image(&pyl_images[active_index]);

                if (active_index == 13)
                {
                    active_index = 0;
                }
                else
                {
                    active_index = active_index + 1;
                }
            }
            else if (joystick_get_pos() == JOYSTICK_POS_DOWN)
            {
                pyl_images[active_index].invert_colors = false;
                pyl_draw_image(&pyl_images[active_index]);
                if (active_index == 0)
                {
                    active_index = 13;
                }
                else
                {
                    active_index = active_index - 1;
                }
            }

            pyl_images[active_index].invert_colors = true;
            pyl_draw_image(&pyl_images[active_index]);
        }

        
        /* If Remote Data has been received, update the remote game info */
        if (ECE353_Events.ipc_rx)
        {
            printf("Remote Data has been received");
            ECE353_Events.ipc_rx = 0;
            if (pyl_ipc_rx(&remote_info))  // Retrieve remote data
            {
                /* Display remote score and spins */
                pyl_print_digits_6(remote_info.score, LOCATION_REMOTE_SCORE_X, LOCATION_REMOTE_SCORE_Y, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
                pyl_print_digits_2(remote_info.spins, LOCATION_REMOTE_SPINS_X, LOCATION_REMOTE_SPINS_Y, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
                pyl_print_digits_2(remote_info.passed, LOCATION_REMOTE_PASSED_X, LOCATION_REMOTE_PASSED_Y, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            }
        }

        /*High Score Updating*/
        if (local_info.score > highScore){
            highScore = local_info.score;
        }

        /*Update EEPROM based on interrupt*/
        // PRESSING BUTTON STOPS EXECUTION :(())
        if (ECE353_Events.io_button == 1){
            ECE353_Events.io_button = 0;
            // Reset local score on the eeprom write, have to split the score by byte into address 0x21 and 0x20
            low_byte = 0x00;
            high_byte = 0x00;
           
            // Reset high score
            highScore = 0;


            eeprom_write_byte(0x20, low_byte);
            eeprom_write_byte(0x21, high_byte);
            //printf("Current High Score: %06d\n\r", pyl_eeprom_high_score_read());

        } 

        if (local_info.spins == 0){
            low_byte = (uint8_t)(highScore & 0x00FF);
            high_byte = (uint8_t)((highScore >> 8));
           
            eeprom_write_byte(0x20, low_byte);
            eeprom_write_byte(0x21, high_byte);
            //printf("Current High Score: %06d\n\r", pyl_eeprom_high_score_read());
        }        
    }
}

#endif
