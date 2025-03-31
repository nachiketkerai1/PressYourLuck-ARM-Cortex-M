/**
 * @file task_remote_rx.c
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2024-08-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "task_remote_rx.h"

#if defined(HW05) || defined(HW06)
#include "main.h"
#include "press-your-luck/pyl.h"
#include "press-your-luck/tasks/task_console.h"

/* Event Groups */
extern EventGroupHandle_t eg_UI;

extern cyhal_uart_t remote_uart_obj;

QueueHandle_t q_Remote_Rx;
extern QueueHandle_t q_Control_Rx_Data;

/******************************************************************************
 * Static Functions
 *
 * These Functions should never be called outside of this file
 ******************************************************************************/

/**
 * @brief
 * Verifies that the data received is valid
 * @param packet
 * An array of 4 bytes received from the remote board
 * @return true
 * Packet is valid
 * @return false
 * Packet contained invalid data
 */
static bool staff_packet_verify(uint8_t *packet)
{
  /* Verify start of packet */
  if (packet[0] != REMOTE_PACKET_START)
  {
    return false;
  }

  /* Verify the command is a a value between 0xB0 and 0xB6*/
  if ((packet[1] < 0xB0) || (packet[1] > 0xB6))
  {
    return false;
  }

  /* Verify end of packet */
  if (packet[6] != REMOTE_PACKET_END)
  {
    return false;
  }

  return true;
}
/******************************************************************************
 * End of Static Functions
 ******************************************************************************/

/* This Global variable is used to store up to 7-bytes of data received on
 * the remote UART. As data is received, add the data to the payload and increment
 * the number of bytes received.
 *
 * Adding the next byte of data
 * Data_Remote_Rx.payload[Data_Remote_Rx.num_bytes] = c;
 *
 * Incrementing the number of bytes received
 * Data_Remote_Tx.num_bytes++;
 *
 */
static remote_uart_data_t Data_Remote_Rx = {.num_bytes = 0, .payload = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

/**
 * @brief
 * This is the interrupt handler for the remote UART. Data is transmitted to the bottom
 * half task using q_Remote_Rx.
 *
 * @param handler_arg
 * @param event
 */
void Handler_FreeRTOS_Remote_UART(void *handler_arg, cyhal_uart_event_t event)
{
  (void)handler_arg;
  uint8_t data;
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

  if ((event & CYHAL_UART_IRQ_TX_ERROR) == CYHAL_UART_IRQ_TX_ERROR)
  {
    /* An error occurred in Tx */
    /* Should never get here */
  }
  else if ((event & CYHAL_UART_IRQ_RX_NOT_EMPTY) == CYHAL_UART_IRQ_RX_NOT_EMPTY)
  {
    /* ADD CODE */
  }
  else if ((event & CYHAL_UART_IRQ_TX_EMPTY) == CYHAL_UART_IRQ_TX_EMPTY)
  {
    /* Should never get here because Tx Empty Interrupts will not be enabled*/
  }
}

/**
 * @brief 
 * Parses the 7-byte packet.  If the packet is invalid, the packet is discarded
 * If the packet is valid, initialize a game_info_msg_t message with the data 
 * received and send it to the q_Control_Rx_Data queue.
 * @param param 
 */
void task_remote_rx_bottom_half(void *param)
{
  /* Suppress warning for unused parameter */
  (void)param;

  /* Repeatedly running part of the task */
  for (;;)
  {
      /* ADD CODE */
  }
}

/**
 * @brief
 * Initializes the Remote UART interface so that interrupts are generated when bytes of data
 * are received by the remote UART.  This function registers a handler and bottom half task
 * that will be used to verify the data received is valid.  As part of the initialization, a
 * FreeRTOS Queue is created so send data between the Interrupt Handler and the bottom half
 * task.
 */
void task_remote_rx_init(void)
{
  q_Remote_Rx = xQueueCreate(1, sizeof(remote_uart_data_t));

  /* Set the remote uart to 115200 8N1*/
  remote_uart_init();
  
  remote_uart_enable_interrupts(
    Handler_FreeRTOS_Remote_UART,
    true, 
    false);

  xTaskCreate(
      task_remote_rx_bottom_half,
      "Task Remote Rx Bottom",
      5 * configMINIMAL_STACK_SIZE,
      NULL,
      ECE353_TASK_PRIORITY_BOTTOM_HALF,
      NULL);
}

#endif
