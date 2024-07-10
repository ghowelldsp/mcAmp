/***********************************************************************************************************************
 *
 * @file    ma12040p_driver.h
 *
 * @brief   MA12040P Driver API's
 *  
 * @par
 * @author  G. Howell
 *
 **********************************************************************************************************************/

#ifndef MA12040P_DRIVER_H
#define MA12040P_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------- INCLUDES ---------------------------------------------------------------*/

#include "ma12040p.h"
#include "drivers/bm_twi_driver/bm_twi.h"

/*------------------------------------------- MACROS AND DEFINES -----------------------------------------------------*/
/*------------------------------------------- TYPEDEFS ---------------------------------------------------------------*/

typedef enum
{
	MA12040P_SUCCESS,
	MA12040P_ERROR
} MA12040P_RESULT;

/*------------------------------------------- EXPORTED VARIABLES -----------------------------------------------------*/
/*------------------------------------------- GLOBAL FUNCTION PROTOTYPES ---------------------------------------------*/

MA12040P_RESULT ma12040p_setVolume(
	BM_TWI *twi,
	float volDb);

MA12040P_RESULT ma12040p_setVlaEnable(
	BM_TWI *twi,
	ma12040p_vlaEnable_t enDi);

MA12040P_RESULT ma12040p_writeReg(
	BM_TWI *twi,
	uint8_t regAddr,
	uint8_t regValue,
	uint8_t regBitMask,
	uint8_t regBitPos);

MA12040P_RESULT ma12040p_readReg(
	BM_TWI *twi,
	uint8_t regAddr,
	uint8_t *regValue);

#ifdef __cplusplus
}
#endif

#endif /* MA12040P_DRIVER_H */
