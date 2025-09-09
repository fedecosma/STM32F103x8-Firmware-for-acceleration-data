/*
 * time_utils.h
 *
 *  Created on: Jul 16, 2025
 *      Author: federicocosma
 *
 *  Used for sampling control: sampling of the sensors and alignment of the latter with sensors' ODR
 */

#ifndef INC_TIME_UTILS_H_
#define INC_TIME_UTILS_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "rtc.h"

extern uint16_t sampling_freq;
extern RTC_DateTypeDef date;
extern RTC_TimeTypeDef time;

void setSamplingFrequency(uint16_t hz);				//sets timer frequency
uint16_t decodeHz(uint8_t data);					//transforms byte to Hz for timer setting
uint8_t decodeODR(uint8_t data);					//transforms byte to byte for coherent sampling of sensors' measurements
													//and for LIS3DHTR_SetODR(uint8_t odr)


void setTimeAndDate(uint8_t day, uint8_t month, uint8_t year, uint8_t hour, uint8_t min, uint8_t sec);	//sets RTC for date and time

#endif /* INC_TIME_UTILS_H_ */
