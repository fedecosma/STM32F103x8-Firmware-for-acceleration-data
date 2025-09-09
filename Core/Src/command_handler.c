/*
 * command_handler.c
 *
 *  Created on: Jul 16, 2025
 *      Author: federicocosma
 */

#include <time_utils.h>
#include "flash_utils.h"
#include "lis3dhtr.h"
#include "usart_io.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"

uint8_t datarate_set = 0x50;				//default data rate = 100Hz
uint8_t fullscale_set = 0x30;				//default full scale = +-16g
uint8_t enabled_axes_set = 0x07;			//default axes enabled = all

uint8_t stream_enabled = 0;					//default stream enabled = no
volatile uint16_t discard_samples = 0;		//discard 2s * Hz samples
uint8_t setup_in_progress = 0;				//setup flag

//used for cmd 36
void handleSetupAccel(uint8_t data_rate, uint8_t full_scale, uint8_t axes)
{
	setup_in_progress = 1;
	discard_samples = 2 * decodeHz(data_rate);

	HAL_TIM_Base_Stop(&htim2);

	//sets data rate
	datarate_set = data_rate;
	fullscale_set = full_scale;
	enabled_axes_set = axes;

	uint16_t hz = decodeHz(data_rate);
	uint8_t odr = decodeODR(data_rate);

	//aligning firmware's interrupt frequency with sensors' measurements reading
	if(LIS3DHTR_SetODR(&hi2c1, odr) != HAL_OK)
		return;
	setSamplingFrequency(hz);

	//sets full scale
	if(LIS3DHTR_SetFullScale(&hi2c1, full_scale) != HAL_OK)
		return;

	//sets enable x, y, z axes
	if(LIS3DHTR_SetAxes(&hi2c1, axes) != HAL_OK)
		return;

	HAL_TIM_Base_Start(&htim2);

	setup_in_progress = 0;

}

void handleCommand(uint8_t *packet)
{
	uint8_t cmd = packet[0];
	uint8_t payload = packet[1];

	//please note that we're just checking the dimensions, not the content of the bytes (TODO)
	switch(cmd)
	{
		//general info
		case 0x35:
			if(payload == 1 && packet[2] == 0)
				Send_Status();
			break;

		//config settings
		case 0x36:
			if (payload == 3)
			{
				handleSetupAccel(packet[2], packet[3], packet[4]);
				Send_ACK(0x36);
			}
			break;

		//start stream
		case 0x37:
			if(payload == 1 && packet[2] == 0x00)
				stream_enabled = 1;
			break;

		//stop stremam
		case 0x38:
			if(payload == 1 && packet[2] == 0x00)
			{
				stream_enabled = 0;
				Send_ACK(0x38);
			}
			break;

		//send one sample
		case 0x39:
			if(payload == 1 && packet[2] == 0x00 && stream_enabled == 0)
				Send_AccelSample_Once();
			break;

		//send A_ms (with date and time)
		case 0x40:
			if(payload == 1 && packet[2] == 0x00)
			{
				Send_MaxA_ms();
			}
			break;

		//send A_mt
		case 0x41:
			if(payload == 1 && packet[2] == 0x00)
			{
				Send_MaxA_mt();
			}
			break;

		//send temperature and humidity
		case 0x42:
			if(payload == 1 && packet[2] == 0x00)
			{
				Send_EnvData();
			}
			break;

		//reset A_ms
		case 0x43:
			if(payload == 1 && packet[2] == 0x00)
			{
				resetA_ms();
				Send_ACK(0x43);
			}
			break;

		//set date and time
		case 0x50:
			if(payload == 6)
			{
				setTimeAndDate(packet[2], packet[3], packet[4], packet[5], packet[6], packet[7]);
				Send_ACK(0x50);
			}
			break;

		default: return; //no action
	}
}


