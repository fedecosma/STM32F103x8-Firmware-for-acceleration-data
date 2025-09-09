/*
 * sensors_data.h
 *
 *  Created on: Jul 16, 2025
 *      Author: federicocosma
 *
 *  Used for defining acceleration, temperature and (relative) humidity data from
 *  the two sensors, as well as for updating the maximum values for A_mt and A_ms.
 *
 *  Note that variables are declared extern because they'll be accessed/modified from other source files, and volatile in order to optimize the compiler,
 *  telling it that such values may change abruptly in an ISR.
 */

#ifndef INC_SENSORS_DATA_H_
#define INC_SENSORS_DATA_H_

#include "lis3dhtr.h"
#include "sht40.h"

extern LIS3DHTR_Data A_i;		//instantaneous acceleration
extern LIS3DHTR_Data A_mt;		//maximum temporary acceleration
extern LIS3DHTR_Data A_ms;		//maximum overall acceleration

extern SHT40_Data env;			//environment data: temperature and humidity

void updateMaxValues(LIS3DHTR_Data *A_i);		//updates max values of A_mt and A_ms

#endif /* INC_SENSORS_DATA_H_ */
