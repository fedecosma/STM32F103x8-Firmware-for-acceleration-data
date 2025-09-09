/*
 * lis3dhtr.h
 *
 *  Created on: Jul 16, 2025
 *      Author: federicocosma
 *
 *  Used for communicating with the LIS3DHTR accelerometer through I2C1 peripheral
 */

#ifndef INC_LIS3DHTR_H_
#define INC_LIS3DHTR_H_

#include "stm32f1xx_hal.h"

#define LIS3DHTR_ADDR			(0x18 << 1) //shifted 7-bit address(0x30) of LIS3DHTR accelerometer
#define LIS3DHTR_WHO_AM_I		0x0F		//who_am_i register address, contains 0x33
#define LIS3DHTR_CTRL_REG_1		0x20		//CTRL_REG1 register address, sets Output Data Rate (ODR)
#define LIS3DHTR_CTRL_REG_4		0x23		//CTRL_REG4 register address, sets Full Scale
#define LIS3DHTR_CTRL_REG_5		0x24		//FIFO buffer control
#define LIS3DHTR_STATUS_REG		0x27		//status register address to check if new data is available
#define LIS3DHTR_OUT_X_L 		0x28		//OUT_X_L register address (increment up to 0x2D), contains accel. data


extern uint8_t ctrl_1;
extern uint8_t ctrl_4;
extern uint8_t whoami;
extern uint8_t data_available;
extern uint8_t buffer[6];
extern uint8_t reg_addr;


typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} LIS3DHTR_Data; //data = two's complement 2 bytes for each axis -> 6 bytes


HAL_StatusTypeDef LIS3DHTR_Init(I2C_HandleTypeDef *hi2c); 										//establishes connection with LIS3DHTR sensor through I2C
HAL_StatusTypeDef LIS3DHTR_Presence(I2C_HandleTypeDef *hi2c);									//check LIS3DHTR presence
int16_t getMaxCounts(uint8_t fs_code);															//returns max values given a full scale setting
float getMgDigit(uint8_t fullscale);
uint8_t LIS3DHTR_DataReady(I2C_HandleTypeDef *hi2c);											//ensures there's new data available
HAL_StatusTypeDef LIS3DHTR_Read(I2C_HandleTypeDef *hi2c, LIS3DHTR_Data *data); 					//reads acceleration data
HAL_StatusTypeDef LIS3DHTR_SetODR(I2C_HandleTypeDef *hi2c, uint8_t odr); 						//sets output data rate
HAL_StatusTypeDef LIS3DHTR_SetFullScale(I2C_HandleTypeDef *hi2c, uint8_t fullscale);			//sets full scale
HAL_StatusTypeDef LIS3DHTR_SetAxes(I2C_HandleTypeDef *hi2c, uint8_t axes);						//sets axes

#endif /* INC_LIS3DHTR_H_ */
