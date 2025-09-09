/*
 * sht40.h
 *
 *  Created on: Jul 16, 2025
 *      Author: federicocosma
 *
 *  Used for communicating with the SHT40 sensor (temperature and relative humidity) through I2C1 peripheral
 */

#ifndef INC_SHT40_H_
#define INC_SHT40_H_

#include "stm32f1xx_hal.h"

#define SHT40_ADDR				(0x44 << 1)		//0x88
#define SHT40_CMD_MEASURE		0xFD 			//High-precision measurement of temperature and relative humidity
#define SHT40_SOFT_RESET_CMD 	0x94;			//soft-reset command

typedef struct{
	uint16_t temperature;
	uint16_t humidity;
} SHT40_Data;

HAL_StatusTypeDef SHT40_Init(I2C_HandleTypeDef *hi2c);							//establishes connection with SHT40 sensor through I2C
HAL_StatusTypeDef SHT40_MeasureRequest(I2C_HandleTypeDef *hi2c);				//sends measure command
HAL_StatusTypeDef SHT40_Read(I2C_HandleTypeDef *hi2c, SHT40_Data *data);		//reads temperature and relative humidity data


#endif /* INC_SHT40_H_ */
