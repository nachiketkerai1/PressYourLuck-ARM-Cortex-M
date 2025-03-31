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

#include "pyl.h"
#include "pyl_handler_tmr_050_ms.h"

#if defined(HW02) || defined(HW03) || defined(HW04)

void handler_timer_050_ms(void *callback_arg, cyhal_timer_event_t event)
{
   
   /* Joystick */
   static joystick_position_t prev_pos = JOYSTICK_POS_CENTER;
   joystick_position_t current_pos = joystick_get_pos();

   if (current_pos != JOYSTICK_POS_CENTER && prev_pos == JOYSTICK_POS_CENTER)
   {
      // Joystick has moved from the center position, set the event
      ECE353_Events.joystick = 1;
   }
   else
   {
      // Joystick is either at center or not moved, clear the event
      ECE353_Events.joystick = 0;
   }

   // Update the previous position to the current position
   prev_pos = current_pos;
}
#endif
