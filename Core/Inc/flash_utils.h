/*
 * flash_utils.h
 *
 *  Created on: Jul 16, 2025
 *      Author: federicocosma
 *
 *  Used for non-volatile memory interaction. Specifically, saving, load and resetting A_ms
 */

#ifndef INC_FLASH_UTILS_H_
#define INC_FLASH_UTILS_H_

#include "rtc.h"

#define FLASH_PAGE_ADDR		0x0800FC00 			//Last page(63) address of the 64KB flash memory of the MCU

extern RTC_DateTypeDef recordedDate;
extern RTC_TimeTypeDef recordedTime;

//void saveA_msToFlash(uint8_t *axes_to_update, uint8_t size);	//saves A_ms
void saveA_msToFlash(void);						//saves A_ms
void loadA_msFromFlash(void);					//loads A_ms
void resetA_ms(void);							//resets A_ms

//void checkJumpToBootloader(void);				TODO
//void jumpToBootloader(void);					TODO

#endif /* INC_FLASH_UTILS_H_ */
