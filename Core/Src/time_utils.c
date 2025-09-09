/*
 * time_utils.c
 *
 *  Created on: Jul 16, 2025
 *      Author: federicocosma
 */

#include <time_utils.h>
#include "tim.h"

uint16_t sampling_freq = 100;			//default frequency
RTC_DateTypeDef date = {0};
RTC_TimeTypeDef time = {0};

void setSamplingFrequency(uint16_t hz)
{
	//avoids out-of-range frequencies
	if((hz == 0) || (hz > 400)) return;

	sampling_freq = hz;

	//clock frequency, prescaler and auto-reload register values
	uint32_t timer_clock = HAL_RCC_GetHCLKFreq();								//72 MHz clock
	uint16_t prescaler = htim2.Init.Prescaler;									//(7199)
	uint32_t timer_freq = timer_clock / (prescaler + 1);						//10 kHz
	uint16_t arr = (timer_freq / hz) - 1;										//Auto-Reload Register value calculation

	__HAL_TIM_DISABLE(&htim2);
	__HAL_TIM_SET_AUTORELOAD(&htim2, arr);
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	__HAL_TIM_ENABLE(&htim2);
}

uint16_t decodeHz(uint8_t data)
{
	switch(data)
	{
		case 0x10: return 1; break;
		case 0x20: return 10; break;
		case 0x30: return 25; break;
		case 0x40: return 50; break;
		case 0x50: return 100; break;
		case 0x60: return 200; break;
		case 0x70: return 400; break;

		default: return 100;
	}
}

uint8_t decodeODR(uint8_t data)
{
	switch(data)
	{
		case 0x10: return 0x01; break;
		case 0x20: return 0x02; break;
		case 0x30: return 0x03; break;
		case 0x40: return 0x04; break;
		case 0x50: return 0x05; break;
		case 0x60: return 0x06; break;
		case 0x70: return 0x07; break;

		default: return 0x05;
	}
}

void setTimeAndDate(uint8_t day, uint8_t month, uint8_t year, uint8_t hour, uint8_t min, uint8_t sec)
{
	date.Date = day;
	date.Month = month;
	date.Year = year;

	time.Hours = hour;
	time.Minutes = min;
	time.Seconds = sec;

	HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);

	HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);

}

