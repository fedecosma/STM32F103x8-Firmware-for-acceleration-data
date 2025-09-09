/*
 * sensors_data.c
 *
 *  Created on: Jul 15, 2025
 *      Author: federicocosma
 */

#include "sensors_data.h"
#include "flash_utils.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

//setting to zero
LIS3DHTR_Data A_i = {0};
LIS3DHTR_Data A_mt = {0};
//LIS3DHTR_Data A_ms = {0};
LIS3DHTR_Data A_ms;

SHT40_Data env = {0};

//updating A_mt and A_ms based off instantaneous acceleration values for each axis
void updateMaxValues(LIS3DHTR_Data *A_i)
{
	//flag to perform save operation of A_ms once for all the axes
	 uint8_t to_update = 0;

	//max A_mt (temporary)
	if(abs(A_i -> x) > abs(A_mt.x)) A_mt.x = abs(A_i -> x);
	if(abs(A_i -> y) > abs(A_mt.y)) A_mt.y = abs(A_i -> y);
	if(abs(A_i -> z) > abs(A_mt.z)) A_mt.z = abs(A_i -> z);

	//max A_ms (persistent)
	if(abs(A_i -> x) > abs(A_ms.x))
	{
		A_ms.x = abs(A_i -> x);
		to_update = 1;
	}

	if(abs(A_i -> y) > abs(A_ms.y))
	{
		A_ms.y = abs(A_i -> y);
		to_update = 1;
	}

	if(abs(A_i -> z) > abs(A_ms.z))
	{
		A_ms.z = abs(A_i -> z);
		to_update = 1;
	}

	//saving A_ms in non-volatile memory
	if(to_update)
	{
		saveA_msToFlash();
		to_update = 0;
	}
}


