/*
 * command_handler.h
 *
 *  Created on: Jul 16, 2025
 *      Author: federicocosma
 *
 *  Used for performing the right actions when command is received, properly crafting the response's structure.
 */

#ifndef INC_COMMAND_HANDLER_H_
#define INC_COMMAND_HANDLER_H_

#include <stdint.h>

extern uint8_t datarate_set;
extern uint8_t fullscale_set;
extern uint8_t enabled_axes_set;
extern uint8_t stream_enabled;
extern volatile uint16_t discard_samples;
extern uint8_t setup_in_progress;

void handleCommand(uint8_t *packet);												//prepares response frame
void handleSetupAccel(uint8_t data_rate, uint8_t full_scale, uint8_t axes);			//performs modification on command 36

#endif /* INC_COMMAND_HANDLER_H_ */
