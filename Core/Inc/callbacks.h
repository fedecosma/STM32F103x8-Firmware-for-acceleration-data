/*
 * callbacks.h
 *
 *  Created on: Aug 6, 2025
 *      Author: federicocosma
 */

#ifndef INC_CALLBACKS_H_
#define INC_CALLBACKS_H_

#include "main.h"

extern volatile uint8_t flag_ms;
extern volatile uint8_t tx_busy;
extern uint8_t rx_byte;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* INC_CALLBACKS_H_ */
