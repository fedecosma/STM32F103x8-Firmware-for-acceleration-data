/*
 * sht40.c
 *
 *  Created on: Jul 15, 2025
 *      Author: federicocosma
 */

#include "sht40.h"

HAL_StatusTypeDef  SHT40_Init(I2C_HandleTypeDef *hi2c)
{
	//soft-reset
	uint8_t cmd = SHT40_SOFT_RESET_CMD;

	//sending command to sensor
	if(HAL_I2C_Master_Transmit(hi2c, SHT40_ADDR, &cmd, 1, HAL_MAX_DELAY) != HAL_OK)
		return HAL_ERROR;

	//wait for reset completion
	HAL_Delay(2);

	return HAL_OK;
}

HAL_StatusTypeDef SHT40_MeasureRequest(I2C_HandleTypeDef *hi2c)
{
	uint8_t cmd = SHT40_CMD_MEASURE;

	if(HAL_I2C_Master_Transmit(hi2c, SHT40_ADDR, &cmd, 1, HAL_MAX_DELAY) != HAL_OK)
		return HAL_ERROR;

	return HAL_OK;
}

HAL_StatusTypeDef SHT40_Read(I2C_HandleTypeDef *hi2c, SHT40_Data *data)
{
	uint8_t rx_buffer[6];	//2 bytes for temperature, 1 for CRC, 2 bytes for humidity, 1 for CRC

	//reading measurements
	if(HAL_I2C_Master_Receive(hi2c, SHT40_ADDR, rx_buffer, 6, HAL_MAX_DELAY) != HAL_OK)
		return HAL_ERROR;

	/*uint16_t raw_temp = (rx_buffer[0] << 8) | rx_buffer[1];					//ignoring CRC byte, MSB first
	data -> temperature = -45.0f + 175.0f * ((float) raw_temp / 65535.0f);		//applying provided conversion formula

	uint16_t raw_humidity = (rx_buffer[3] << 8) | rx_buffer[4];					//ignoring CRC byte, MSB first
	data -> humidity = -6.0f + 125.0f * ((float) raw_humidity / 65535.0f);		//applying provided conversion formula (note that function exceeds 0% and 100%

	//fixing out-of-range conversions
	if (data -> humidity < 0.0f) data -> humidity = 0.0f;
	if (data -> humidity >100.0f) data -> humidity = 100.0f;*/

	data -> temperature = (uint16_t)(rx_buffer[0] << 8) | rx_buffer[1];					//raw temperature (before applying formula, ignore CRC, MSB first)
	data -> humidity = (uint16_t)(rx_buffer[3] << 8) | rx_buffer[4];					//raw humidity (before applying formula, ignore CRC, MSB first)

	return HAL_OK;
}

