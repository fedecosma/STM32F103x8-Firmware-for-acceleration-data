/*
 * lis3dhtr.c
 *
 *  Created on: Jul 14, 2025
 *      Author: federicocosma
 */

#include "lis3dhtr.h"
#include "command_handler.h"
#include <string.h>

uint8_t ctrl_1 = 0x57;	//ODR[3:0] = '0101' (100Hz default), '0' (HighRes mode), (Z enable, Y enable, X enable) = '111' -> 0101 0111
uint8_t ctrl_4 = 0x38;	//BDU = '0' (continuous update), Little Endian = '0', Full scale[1:0] = 11 (+-16g), HighRes enabled = '1', Self-test = '00', SPI = '0' -> 0011 1000
uint8_t whoami = 0;		//WHO_AM_I register content
uint8_t data_available = 0;	//data ready
uint8_t buffer[6] = {0}; 		//16-bit output for each axis (12-bit output left-justified)
uint8_t reg_addr = LIS3DHTR_OUT_X_L | 0x80; //auto increment function of LIS3DHTR sensor (bit #7LSb = 1)

HAL_StatusTypeDef LIS3DHTR_Init(I2C_HandleTypeDef *hi2c)
{
	//writes to CTRL_REG1
	if(HAL_I2C_Mem_Write(hi2c, LIS3DHTR_ADDR, LIS3DHTR_CTRL_REG_1, 1, &ctrl_1, 1, HAL_MAX_DELAY) != HAL_OK)
		return HAL_ERROR;

	//writes to CTRL_REG4
	if(HAL_I2C_Mem_Write(hi2c, LIS3DHTR_ADDR, LIS3DHTR_CTRL_REG_4, 1, &ctrl_4, 1, HAL_MAX_DELAY) != HAL_OK)
		return HAL_ERROR;

	//FIFO enable
	/*uint8_t ctrl_5 = 0;
	if(HAL_I2C_Mem_Read(hi2c, LIS3DHTR_ADDR, LIS3DHTR_CTRL_REG_5, 1, &ctrl_5, 1, HAL_MAX_DELAY) != HAL_OK)
		return HAL_ERROR;
	ctrl_5 = ctrl_5 & ~(1 << 6);
	if(HAL_I2C_Mem_Write(hi2c, LIS3DHTR_ADDR, LIS3DHTR_CTRL_REG_5, 1, &ctrl_5, 1, HAL_MAX_DELAY) != HAL_OK)
		return HAL_ERROR;*/

	//WHO_AM_I check
	if(LIS3DHTR_Presence(hi2c) != HAL_OK)
		return HAL_ERROR;

	return HAL_OK;
}

HAL_StatusTypeDef LIS3DHTR_Presence(I2C_HandleTypeDef *hi2c)
{
	if(HAL_I2C_Mem_Read(hi2c, LIS3DHTR_ADDR, LIS3DHTR_WHO_AM_I, 1, &whoami, 1, HAL_MAX_DELAY) != HAL_OK)
		return HAL_ERROR;

	if(whoami != 0x33)
		return HAL_ERROR;

	return HAL_OK;
}

int16_t getMaxCounts(uint8_t fs_code)
{
	switch(fs_code)
	{
		case 0x00: return 32000; break;		//1 mg/LSB --> raw_bytes = (2 * 16) / 1mg
		case 0x10: return 32000; break;		//2 mg/LSB --> raw_bytes = (4 * 16) / 2mg
		case 0x20: return 32000; break;		//4 mg/LSB --> raw_bytes = (8 * 16) / 4mg
		case 0x30: return 21333; break;		//12 mg/LSB --> raw_bytes = (16 * 16) / 12mg

		default: return 32000;
	}
}

float getMgDigit(uint8_t fullscale)
{
	switch(fullscale)
	{
	case 0x00: return 0.001f; break;
	case 0x10: return 0.002f; break;
	case 0x20: return 0.004f; break;
	case 0x30: return 0.012f; break;

	default: return 0.012f;
	}
}

int16_t getMaxValues(uint8_t fullscale)
{
	switch(fullscale)
	{
	case 0x00: return 2000; break;
	case 0x10: return 4000; break;
	case 0x20: return 8000; break;
	case 0x30: return 16000; break;

	default: return 16000;
	}
}

uint8_t LIS3DHTR_DataReady(I2C_HandleTypeDef *hi2c)
{
	if(HAL_I2C_Mem_Read(hi2c, LIS3DHTR_ADDR, LIS3DHTR_STATUS_REG, I2C_MEMADD_SIZE_8BIT, &data_available, 1, HAL_MAX_DELAY) != HAL_OK)
		return 0;

	return (data_available & 0x08);	//bit #3
}

HAL_StatusTypeDef LIS3DHTR_Read(I2C_HandleTypeDef *hi2c, LIS3DHTR_Data *data)
{
	int16_t raw_x = 0, raw_y = 0, raw_z = 0;

	if(HAL_I2C_Mem_Read(hi2c, LIS3DHTR_ADDR, reg_addr, 1, buffer, 6, HAL_MAX_DELAY) != HAL_OK)
		return HAL_ERROR;

	//discards disabled axes measurements and sets them to zero
	if(enabled_axes_set & 0x01)
		raw_x = (int16_t)(buffer[1] << 8 | buffer[0]);
	if(enabled_axes_set & 0x02)
		raw_y = (int16_t)(buffer[3] << 8 | buffer[2]);
	if(enabled_axes_set & 0x04)
		raw_z = (int16_t)(buffer[5] << 8 | buffer[4]);

	//checking full scale overflows

	//int16_t max_counts = getMaxValues(fullscale_set);

	float x_g = ((float)raw_x * getMgDigit(fullscale_set)) / 16.0f;
	data -> x = (int16_t)(x_g * 1000);
	//if(data -> x > max_counts)  data -> x = max_counts;
	//if(data -> x < -max_counts) data -> x = -max_counts;

	float y_g = ((float)raw_y * getMgDigit(fullscale_set)) / 16.0f;
	data -> y = (int16_t)(y_g * 1000);
	//if(data -> y > max_counts)  data -> y = max_counts;
	//if(data -> y < -max_counts) data -> y = -max_counts;

	float z_g = ((float)raw_z * getMgDigit(fullscale_set)) / 16.0f;
	data -> z = (int16_t)(z_g * 1000);
	//if(data -> z > max_counts)  data -> z = max_counts;
	//if(data -> z < -max_counts) data -> z = -max_counts;

	//memset(buffer, 0, sizeof(buffer));

	return HAL_OK;
}

HAL_StatusTypeDef LIS3DHTR_SetODR(I2C_HandleTypeDef *hi2c, uint8_t odr)
{
	//reads CTRL_REG1 value
	if(HAL_I2C_Mem_Read(hi2c, LIS3DHTR_ADDR, LIS3DHTR_CTRL_REG_1, 1, &ctrl_1, 1, HAL_MAX_DELAY) != HAL_OK)
		return HAL_ERROR;

	ctrl_1 = ctrl_1 & 0x0F; 					//masks ODR bits: ODR[3:0] = '0000'
	ctrl_1 = ctrl_1 | (odr << 4); 				//places new ODR bits

	//writes to CTRL_REG1
	return HAL_I2C_Mem_Write(hi2c, LIS3DHTR_ADDR, LIS3DHTR_CTRL_REG_1, 1, &ctrl_1, 1, HAL_MAX_DELAY);

}

HAL_StatusTypeDef LIS3DHTR_SetFullScale(I2C_HandleTypeDef *hi2c, uint8_t fullscale)
{
	if(fullscale > 0x30)
		return HAL_ERROR;

	//reads CTRL_REG4 value
	if(HAL_I2C_Mem_Read(hi2c, LIS3DHTR_ADDR, LIS3DHTR_CTRL_REG_4, 1, &ctrl_4, 1, HAL_MAX_DELAY) != HAL_OK)
		return HAL_ERROR;

	ctrl_4 = ctrl_4 & ~(0x30); 					//clears full scale[1:0]
	ctrl_4 = ctrl_4 | (fullscale & 0x30); 		//sets new bits

	//writes to CTRL_REG4
	return HAL_I2C_Mem_Write(hi2c, LIS3DHTR_ADDR, LIS3DHTR_CTRL_REG_4, 1, &ctrl_4, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef LIS3DHTR_SetAxes(I2C_HandleTypeDef *hi2c, uint8_t axes)
{
	if(axes > 0x07)
		return HAL_ERROR;

	//reads CTRL_REG1 value
	if(HAL_I2C_Mem_Read(hi2c, LIS3DHTR_ADDR, LIS3DHTR_CTRL_REG_1, 1, &ctrl_1, 1, HAL_MAX_DELAY) != HAL_OK)
		return HAL_ERROR;

	ctrl_1 = ctrl_1 & 0xF0; 					//clears Xen,Yen,Zen bits
	ctrl_1 = ctrl_1 | (axes & 0x07); 			//sets new bits

	//writes to CTRL_REG1
	return HAL_I2C_Mem_Write(hi2c, LIS3DHTR_ADDR, LIS3DHTR_CTRL_REG_1, 1, &ctrl_1, 1, HAL_MAX_DELAY);

}

