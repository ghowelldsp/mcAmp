/***********************************************************************************************************************
 *
 * @file    ma12040p_driver.c
 *
 * @brief   MA12040P Driver
 *  
 * @par
 * @author  G. Howell
 *
 **********************************************************************************************************************/

/*------------------------------------------- INCLUDES ---------------------------------------------------------------*/

#include "ma12040p_driver.h"

/*------------------------------------------- EXTERN VARIABLES -------------------------------------------------------*/
/*------------------------------------------- PRIVATE MACROS AND DEFINES ---------------------------------------------*/
/*------------------------------------------- PRIVATE TYPEDEFS -------------------------------------------------------*/
/*------------------------------------------- STATIC VARIABLES -------------------------------------------------------*/
/*------------------------------------------- GLOBAL VARIABLES -------------------------------------------------------*/
/*------------------------------------------- STATIC FUNCTION PROTOTYPES ---------------------------------------------*/
/*------------------------------------------- STATIC FUNCTIONS -------------------------------------------------------*/
/*------------------------------------------- GLOBAL FUNCTIONS -------------------------------------------------------*/

MA12040P_RESULT ma12040p_writeReg(
	BM_TWI *twi,
	uint8_t regAddr,
	uint8_t regValue)
{
	uint8_t rdData;
	uint8_t wrData[2] = {regAddr, regValue};

	// write param data
	if (twi_write_block_r(twi, wrData, 2U, false) != TWI_SIMPLE_SUCCESS)
	{
		return MA12040P_ERROR;
	}

	// read back the value to check it has been written correctly
	if (ma12040p_readReg(twi, wrData[0], &rdData) != MA12040P_SUCCESS)
	{
		return MA12040P_ERROR;
	}
	else
	{
		if (rdData != regValue)
		{
			return MA12040P_ERROR;
		}
	}

	return MA12040P_SUCCESS;
}

MA12040P_RESULT ma12040p_readReg(
	BM_TWI *twi,
	uint8_t regAddr,
	uint8_t *regValue)
{
	// write register address to the device
	if (twi_write_block_r(twi, &regAddr, 1U, false) != TWI_SIMPLE_SUCCESS)
	{
		return MA12040P_ERROR;
	}

	// read the register address data sent back from the device
	if (twi_read(twi, regValue) != TWI_SIMPLE_SUCCESS)
	{
		return MA12040P_ERROR;
	}

	return MA12040P_SUCCESS;
}
