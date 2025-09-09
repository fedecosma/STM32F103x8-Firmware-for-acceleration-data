/*
 * serial_parser.h
 *
 *  Created on: Jul 16, 2025
 *      Author: federicocosma
 *
 *  Used for processing each byte of the commands that are being sent
 */

#include <stdint.h>

#ifndef INC_SERIAL_PARSER_H_
#define INC_SERIAL_PARSER_H_

#define MAX_PACKET_DIM		32				//32 bytes maximum

typedef struct{
	uint8_t buffer[MAX_PACKET_DIM];
	uint8_t index;
} Parser;


void Parser_Init(void);						//initializes parser
void Parser_ProcessByte(uint8_t byte);		//processes each byte and performs additional checks

#endif /* INC_SERIAL_PARSER_H_ */
