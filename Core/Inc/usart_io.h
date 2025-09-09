/*
 * usart_io.h
 *
 *  Created on: Jul 16, 2025
 *      Author: federicocosma
 *
 *  Used for sending data to
 */

#ifndef INC_USART_IO_H_
#define INC_USART_IO_H_

#include <stdint.h>

uint8_t computeCRC(const uint8_t* data, uint8_t len);	//computes crc (to be precise it is a checksum)

void Send_ACK(uint8_t cmd_byte);			//sends acknowledgment to no-response commands
void Send_Status(void);						//cmd 35
void Send_AccelSample(void);				//cmd 37
void Send_AccelSample_Once(void);			//cmd 39
void Send_MaxA_ms(void);					//cmd 40
void Send_MaxA_mt(void);					//cmd 41
void Send_EnvData(void);					//cmd 42

#endif /* INC_USART_IO_H_ */
