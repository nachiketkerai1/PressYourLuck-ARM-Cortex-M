/**
 * @file remote_uart_tx.c
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief
 * @version 0.1
 * @date 2023-09-28
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "remote_uart_tx.h"
#include "cy_syslib.h"
#include <sys/types.h>
/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
extern cyhal_uart_t remote_uart_obj;

/*******************************************************************************
 * Function Declarations
 ******************************************************************************/

Circular_Buffer *Tx_Circular_Buffer;

/**
 * @brief
 * Send a character to the remote UART
 * @param
 * Character to Send
 */
void remote_uart_tx_char_async(char c)
{
    /* ADD CODE */

    /* Wait while the circular buffer is full*/
    while (circular_buffer_full(Tx_Circular_Buffer)){

    }

    /* Disable interrupts -- Disable NVIC */
    __disable_irq();
    
    /* Add the current character*/
    cyhal_uart_putc(&remote_uart_obj, c);

    /* Re-enable interrupts */
    __enable_irq();

    /* Enable the Transmit Empty Interrupts*/
    cyhal_uart_enable_event(&remote_uart_obj, (cyhal_uart_event_t)(CYHAL_UART_IRQ_TX_EMPTY), 7, true);
}

/**
 * @brief
 * Prints out a NULL terminated string to the remote UART
 * @param msg
 * String to print
 */
void remote_uart_tx_data_async(char *msg, uint16_t num_bytes)
{
    /* ADD CODE */

    /* check to see that msg pointer is not equal to NULL*/
    uint8_t i = 0;
    /* Add characters to the circular buffer until we reach the NULL character*/
    while (num_bytes > 0)
    {
        /* Wait while the circular buffer is full*/

        /* Disable interrupts -- Disable NVIC */
        __disable_irq();

        /* Add the current character*/
        circular_buffer_add(Tx_Circular_Buffer, msg[i]);

        /* Re-enable interrupts */
        __enable_irq();

        /* Go to the next character*/
        i++;

        /* Decrement the number of bytes left to transfer */
        num_bytes--;
    }

    /* Enable the Transmit Empty Interrupts*/
    cyhal_uart_enable_event(&remote_uart_obj, (cyhal_uart_event_t)(CYHAL_UART_IRQ_TX_EMPTY), 7, true);

}

/**
 * @brief
 * Initializes the Circular Buffer used for transmitting characters to the remote UART
 * and will disable Tx Empty interrupts.
 */
void remote_uart_tx_interrupts_init(void)
{
    /* Initialize the Tx Circular Buffer */
    Tx_Circular_Buffer = circular_buffer_init(128);


    /* Turn Off Tx Interrupts*/
    cyhal_uart_enable_event(&remote_uart_obj,CYHAL_UART_IRQ_TX_EMPTY, 7, false);
}

/**
 * @brief
 * Implements the Tx portion of the UART Handler
 */
void remote_uart_event_handler_process_tx(void)
{
    /* The UART finished transferring data, so check and see if
     * the circular buffer is empty*/
    if (circular_buffer_empty(Tx_Circular_Buffer))
    {
        /* No More data to send, so disable Tx Empty Interrupts
         * to avoid the UART handler from constantly being
         * called
         */
        cyhal_uart_enable_event(&remote_uart_obj,CYHAL_UART_IRQ_TX_EMPTY, 7, false);


        /* Disable the Transmit Empty Interrupts*/
        cyhal_uart_enable_event(&remote_uart_obj, (cyhal_uart_event_t)(CYHAL_UART_IRQ_TX_EMPTY), 7, false);

    }
    else
    {
        /* Transmit the next character in the circular buffer */
        uint32_t c = circular_buffer_remove(Tx_Circular_Buffer);
        cyhal_uart_putc(&remote_uart_obj, c);

    }
}
