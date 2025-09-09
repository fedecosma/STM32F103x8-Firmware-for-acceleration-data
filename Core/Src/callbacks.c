/*
 * callbacks.c
 *
 *  Created on: Aug 6, 2025
 *      Author: federicocosma
 */

#include <time_utils.h>
#include "callbacks.h"
#include "lis3dhtr.h"
#include "sht40.h"
#include "sensors_data.h"
#include "flash_utils.h"
#include "command_handler.h"
#include "usart_io.h"
#include "serial_parser.h"
#include "i2c.h"
#include "usart.h"

//Timer callback
static uint16_t counter_measure = 0;
static uint16_t counter_read = 0;
uint8_t time_between_requests = 3;
uint8_t time_between_readings = 4;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim -> Instance == TIM2)
	{
		if(!setup_in_progress)		//interrupt called and not in parameters setting (i.e command 36)
		{

			if(discard_samples)
			{
				discard_samples--;
			}
			else
			{
				if(LIS3DHTR_DataReady(&hi2c1))	//data available
				{
					if(LIS3DHTR_Read(&hi2c1, &A_i) == HAL_OK)		//reading of LIS3DHTR sensor
					{
						updateMaxValues(&A_i);					//updating A_mt and A_ms
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
					}
				}

				if(++counter_measure >= time_between_requests * sampling_freq)	//0.33Hz measure request
				{
					counter_measure = 0;
					SHT40_MeasureRequest(&hi2c1);
				}

				if(++counter_read >= time_between_readings * sampling_freq) 	//0.25 Hz reading of SHT40 sensor
				{
					counter_read = 0;
					SHT40_Read(&hi2c1, &env);
					HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
				}

				if(stream_enabled)		//automatically sends samples if stream is enabled
				{
					Send_AccelSample();
				}
			}
		}
	}

}


//UART DMA transmission callback
volatile uint8_t tx_busy;
uint8_t rx_byte;
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart -> Instance == USART1)
		tx_busy = 0;
}

//UART DMA reception callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart -> Instance == USART1)
	{
		Parser_ProcessByte(rx_byte);			   //transfers byte to parser
		HAL_UART_Receive_DMA(&huart1, &rx_byte, 1); //restarts rx for next byte
	}
}

