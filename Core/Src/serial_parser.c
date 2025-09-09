/*
 * serial_parser.c
 *
 *  Created on: Jul 16, 2025
 *      Author: federicocosma
 */

#include "serial_parser.h"
#include "command_handler.h"
#include <string.h>

static Parser parser;

void Parser_Init(void)
{
	memset(&parser, 0, sizeof(Parser)); //resets parser (index = 0, buffer[] = {0})
}

void Parser_ProcessByte(uint8_t byte)
{
	//overflow check
	if(parser.index >= MAX_PACKET_DIM)
		Parser_Init(); //reset

	parser.buffer[parser.index++] = byte;

	//cmd and payload bytes in buffer
	if(parser.index == 2)
	{
		if(parser.buffer[1] > (MAX_PACKET_DIM - 3)) //checks if payload is bigger than MAX_PACKET_DIM (exluding cmd,payload and crc bytes)
		{
			Parser_Init(); //resets
			return;
		}
	}

	//check if #data bytes corresponds to what declared in payload byte? TODO

	//4 bytes minimum: command | payload | data | CRC
	if(parser.index > 3 && parser.index == (parser.buffer[1] + 3)) //reached termination assuming #data bytes is coherent
	{
		//crc
		uint8_t crc = 0;
		for(uint8_t i = 0; i < parser.index - 1; i++)
			crc += parser.buffer[i];

		//checks CRC
		if(crc == parser.buffer[parser.index - 1])
			handleCommand(parser.buffer); //transfers command sequence to the handler

		Parser_Init(); //resets once full command is dispatched
	}

}

