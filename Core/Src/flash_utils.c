 /*
 * saveToFlash.c
 *
 *  Created on: Jul 15, 2025
 *      Author: federicocosma
 */

#include "flash_utils.h"
#include "sensors_data.h"
#include <stdint.h>
#include <time_utils.h>

RTC_DateTypeDef recordedDate = {0};
RTC_TimeTypeDef recordedTime = {0};

/*void saveA_msToFlash(uint8_t *axes_to_update, uint8_t size)
{
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	//unlocks flash memory
	HAL_FLASH_Unlock();

	//deletes page
	FLASH_EraseInitTypeDef erase;
	erase.TypeErase  = FLASH_TYPEERASE_PAGES;
	erase.PageAddress = FLASH_PAGE_ADDR;
	erase.NbPages = 1;
	uint32_t pageErr = 0;

	//checks if deletion was successful, otherwise locks flash memory and return
	if(HAL_FLASHEx_Erase(&erase, &pageErr) != HAL_OK)
	{
		HAL_FLASH_Lock();
		return;
	}

	//saving A_ms (x,y,z)
	for(uint8_t i = 0; i < size; i++)
	{
		if(axes_to_update[i])
		{
			uint16_t axis = (i == 0) ? (uint16_t)A_ms.x : (i == 1) ? (uint16_t)A_ms.y : (uint16_t)A_ms.z;
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, FLASH_PAGE_ADDR + (i*2), axis);
		}
	}

	//saving date and time
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, FLASH_PAGE_ADDR + 6, (uint16_t) (date.Date) | ((uint16_t)date.Month << 8));
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, FLASH_PAGE_ADDR + 8, (uint16_t) (date.Year));
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, FLASH_PAGE_ADDR + 10,(uint16_t) (time.Hours) | ((uint16_t)time.Minutes << 8));
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, FLASH_PAGE_ADDR + 12,(uint16_t) (time.Seconds));

	//locks flash memory
	HAL_FLASH_Lock();

	//saving registered date
	recordedDate.Date = date.Date;
	recordedDate.Month = date.Month;
	recordedDate.Year = date.Year;

	recordedTime.Hours = time.Hours;
	recordedTime.Minutes = time.Minutes;
	recordedTime.Seconds = time.Seconds;
}
*/

void saveA_msToFlash(void)
{
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	//unlocks flash memory
	HAL_FLASH_Unlock();

	//deletes page
	FLASH_EraseInitTypeDef erase;
	erase.TypeErase  = FLASH_TYPEERASE_PAGES;
	erase.PageAddress = FLASH_PAGE_ADDR;
	erase.NbPages = 1;
	uint32_t pageErr = 0;

	if(HAL_FLASHEx_Erase(&erase, &pageErr) != HAL_OK)
	{
		HAL_FLASH_Lock();
		return;
	}

	//saves A_ms.x, y, z
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, FLASH_PAGE_ADDR + 0, (uint16_t)A_ms.x);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, FLASH_PAGE_ADDR + 2, (uint16_t)A_ms.y);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, FLASH_PAGE_ADDR + 4, (uint16_t)A_ms.z);

	//saves last date and time
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, FLASH_PAGE_ADDR + 6, (uint16_t)(date.Date) | ((uint16_t)date.Month << 8));
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, FLASH_PAGE_ADDR + 8, (uint16_t)(date.Year));
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, FLASH_PAGE_ADDR + 10, (uint16_t)(time.Hours) | ((uint16_t)time.Minutes << 8));
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, FLASH_PAGE_ADDR + 12, (uint16_t)(time.Seconds));

	//locks flash
	HAL_FLASH_Lock();

	//saving registered date
	recordedDate.Date = date.Date;
	recordedDate.Month = date.Month;
	recordedDate.Year = date.Year;

	recordedTime.Hours = time.Hours;
	recordedTime.Minutes = time.Minutes;
	recordedTime.Seconds = time.Seconds;
}

void loadA_msFromFlash(void)
{
	A_ms.x = *(int16_t*) (FLASH_PAGE_ADDR);
	A_ms.y = *(int16_t*) (FLASH_PAGE_ADDR + 2);
	A_ms.z = *(int16_t*) (FLASH_PAGE_ADDR + 4);

	uint16_t dateVal = *(uint16_t*) (FLASH_PAGE_ADDR + 6);
	recordedDate.Date = dateVal & 0xFF;
	recordedDate.Month = (dateVal >> 8) & 0xFF;
	recordedDate.Year = *(uint8_t*) (FLASH_PAGE_ADDR + 8);

	uint16_t timeVal = *(uint16_t*) (FLASH_PAGE_ADDR + 10);
	recordedTime.Hours = timeVal & 0xFF;
	recordedTime.Minutes = (timeVal >> 8) & 0xFF;
	recordedTime.Seconds = *(uint8_t*) (FLASH_PAGE_ADDR + 12);
}

void resetA_ms(void)
{
	A_ms.x = 0.0f;
	A_ms.y = 0.0f;
	A_ms.z = 0.0f;

	saveA_msToFlash();
}


