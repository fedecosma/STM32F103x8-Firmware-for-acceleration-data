/*
 * usart_io.c
 *
 *  Created on: Jul 16, 2025
 *      Author: federicocosma
 */
#include "usart.h"
#include "i2c.h"
#include "sensors_data.h"
#include "command_handler.h"
#include "flash_utils.h"
#include "callbacks.h"


uint8_t computeCRC(const uint8_t* data, uint8_t len)
{
	uint8_t crc = 0;

	for(uint8_t i = 0; i < (len - 1); i++)
		crc += data[i];	//overflows on purpose (it is a mod 256 checksum)

	return crc;
}

void Send_ACK(uint8_t cmd_byte)
{
	static uint8_t frame[4];

	frame[0] = cmd_byte + 0x80;
	frame[1] = 0x01;
	frame[2] = 0x00;
	frame[3] = (frame[0] + frame[1] + frame[2]) & 0xFF;

	tx_busy = 1;
	HAL_UART_Transmit_DMA(&huart1, frame, sizeof(frame));
}

void Send_Status(void)
{
	static uint8_t frame[18];
	//int16_t temp = (int16_t)(T_i * 10.0f); //useful in order to avoid float types, using int
	//int16_t humidity = (int16_t)(H_i * 10.0f);

	frame[0] = 0x35 + 0x80;
	frame[1] = 0x0F;

	//checks if LIS3DHTR is present
	frame[2] = ((whoami == 0x33) ? 0x01 : 0x00);

	frame[3] = (uint8_t)(env.temperature & 0xFF);				//LSB
	frame[4] = (uint8_t)((env.temperature >> 8) & 0xFF);		//MSB

	frame[5] = (uint8_t)(env.humidity & 0xFF);
	frame[6] = (uint8_t)((env.humidity >> 8) & 0xFF);

	frame[7] = fullscale_set;
	frame[8] = datarate_set;
	frame[9] = enabled_axes_set;
	frame[10] = (stream_enabled ? 0x01 : 0x00);

	//A_mt or A_ms?
	frame[11] = (uint8_t)(A_ms.x & 0xFF);
	frame[12] = (uint8_t)((A_ms.x >> 8) & 0xFF);

	frame[13] = (uint8_t)(A_ms.y & 0xFF);
	frame[14] = (uint8_t)((A_ms.y >> 8) & 0xFF);

	frame[15] = (uint8_t)(A_ms.z & 0xFF);
	frame[16] = (uint8_t)((A_ms.z >> 8) & 0xFF);

	frame[17] = computeCRC(frame, sizeof(frame));

	tx_busy = 1;
	HAL_UART_Transmit_DMA(&huart1, frame, sizeof(frame));
}

void Send_AccelSample(void)
{
	static uint8_t frame[9];

	frame[0] = 0x37 + 0x80;
	frame[1] = 0x06;

	frame[2] = (uint8_t)(A_i.x & 0xFF);
	frame[3] = (uint8_t)((A_i.x >> 8) & 0xFF);

	frame[4] = (uint8_t)(A_i.y & 0xFF);
	frame[5] = (uint8_t)((A_i.y >> 8) & 0xFF);

	frame[6] = (uint8_t)(A_i.z & 0xFF);
	frame[7] = (uint8_t)((A_i.z >> 8) & 0xFF);

	frame[8] = computeCRC(frame, sizeof(frame));;

	tx_busy = 1;
	HAL_UART_Transmit_DMA(&huart1, frame, sizeof(frame));
}

void Send_AccelSample_Once(void)
{
	static uint8_t frame[9];

	frame[0] = 0x39 + 0x80;
	frame[1] = 0x06;

	frame[2] = (uint8_t)(A_i.x & 0xFF);
	frame[3] = (uint8_t)((A_i.x >> 8) & 0xFF);

	frame[4] = (uint8_t)(A_i.y & 0xFF);
	frame[5] = (uint8_t)((A_i.y >> 8) & 0xFF);

	frame[6] = (uint8_t)(A_i.z & 0xFF);
	frame[7] = (uint8_t)((A_i.z >> 8) & 0xFF);

	frame[8] = computeCRC(frame, sizeof(frame));;

	tx_busy = 1;
	HAL_UART_Transmit_DMA(&huart1, frame, sizeof(frame));
}

void Send_MaxA_ms(void)
{
	static uint8_t frame[15];

	frame[0] = 0x40 + 0x80;
	frame[1] = 0x0C;

	frame[2] = (uint8_t)(A_ms.x & 0xFF);
	frame[3] = (uint8_t)((A_ms.x >> 8) & 0xFF);

	frame[4] = (uint8_t)(A_ms.y & 0xFF);
	frame[5] = (uint8_t)((A_ms.y >> 8) & 0xFF);

	frame[6] = (uint8_t)(A_ms.z & 0xFF);
	frame[7] = (uint8_t)((A_ms.z >> 8) & 0xFF);

	frame[8] = (uint8_t)(recordedDate.Date);
	frame[9] = (uint8_t)(recordedDate.Month);
	frame[10] = (uint8_t)(recordedDate.Year);

	frame[11] = (uint8_t)(recordedTime.Hours);
	frame[12] = (uint8_t)(recordedTime.Minutes);
	frame[13] = (uint8_t)(recordedTime.Seconds);

	frame[14] = computeCRC(frame, sizeof(frame));

	tx_busy = 1;
	HAL_UART_Transmit_DMA(&huart1, frame, sizeof(frame));
}

void Send_MaxA_mt(void)
{
	static uint8_t frame[9];

	frame[0] = 0x41 + 0x80;
	frame[1] = 0x06;

	frame[2] = (uint8_t)(A_mt.x & 0xFF);
	frame[3] = (uint8_t)((A_mt.x >> 8) & 0xFF);

	frame[4] = (uint8_t)(A_mt.y & 0xFF);
	frame[5] = (uint8_t)((A_mt.y >> 8) & 0xFF);

	frame[6] = (uint8_t)(A_mt.z & 0xFF);
	frame[7] = (uint8_t)((A_mt.z >> 8) & 0xFF);

	frame[8] = computeCRC(frame, sizeof(frame));

	tx_busy = 1;
	HAL_UART_Transmit_DMA(&huart1, frame, sizeof(frame));
}

void Send_EnvData(void)
{
	static uint8_t frame[7];

	frame[0] = 0x42 + 0x80;
	frame[1] = 0x04;

	frame[2] = (uint8_t)(env.temperature & 0xFF);				//LSB
	frame[3] = (uint8_t)((env.temperature >> 8) & 0xFF);		//MSB

	frame[4] = (uint8_t)(env.humidity & 0xFF);
	frame[5] = (uint8_t)((env.humidity >> 8) & 0xFF);

	frame[6] = computeCRC(frame, sizeof(frame));

	tx_busy = 1;
	HAL_UART_Transmit_DMA(&huart1, frame, sizeof(frame));
}
