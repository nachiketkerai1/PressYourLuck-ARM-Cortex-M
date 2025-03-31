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
#include "task_eeprom.h"
#include "ece353_tasks.h"
#include "portmacro.h"

#if defined(HW05) || defined (HW06)
#include "main.h"
#include "press-your-luck/pyl.h"
#include "press-your-luck/tasks/task_console.h"

/* Event Groups */
extern EventGroupHandle_t eg_UI;

QueueHandle_t q_EEPROM;

/**
 * @brief 
 * Displays the number of whammies that have been selected
 * @param param 
 * Not Used
 */
void task_eeprom(void *param)
{
    eeprom_msg_t msg;
    /* Suppress warning for unused parameter */
    (void)param;

    /* Repeatedly running part of the task */
    for (;;)
    {
        if (xQueueReceive(q_EEPROM, &msg, portMAX_DELAY)) { // Receive the queue
            switch(msg.operation) {
                case EEPROM_WRITE:
                    uint8_t topBits = (msg.score >> 8) & 0xFF; // Set top bits
                    uint8_t bottomBits = msg.score & 0xFF; // Set bottom bits 
                    // Wire to the eeprom
                    eeprom_write_byte(0x20, topBits);
                    eeprom_write_byte(0x21, bottomBits);
                    break;
                case EEPROM_READ:
                    uint8_t tBits = eeprom_read_byte(0x20); // Set top bits
                    uint8_t bBits = eeprom_read_byte(0x21); // Set bottom bits
                    uint16_t score = (tBits << 8) | bBits; // Create score packet 

                    eeprom_msg_t response = {
                        .operation = EEPROM_READ,
                        .score = score,
                        .return_queue = NULL // No need for return_queue in the response
                    };

                    xQueueSend(msg.return_queue, &response, portMAX_DELAY); // Send the response for a read
                    break;
            }
        }
    }
}

void task_eeprom_init(void)
{
    /* Enable the SPI interface */
    spi_init();

    /* Configure the IO pin used to control the chip select*/
    eeprom_cs_init();

    q_EEPROM = xQueueCreate(1, sizeof(eeprom_msg_t));

    xTaskCreate(
        task_eeprom,
        "Task EEPROM",
        configMINIMAL_STACK_SIZE,
        NULL,
        ECE353_TASK_PRIORITY_UI_INPUT,
        NULL);
}

#endif
