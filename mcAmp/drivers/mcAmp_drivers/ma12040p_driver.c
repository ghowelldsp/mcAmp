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

#include <math.h>

#include "ma12040p_driver.h"

/*------------------------------------------- EXTERN VARIABLES -------------------------------------------------------*/
/*------------------------------------------- PRIVATE MACROS AND DEFINES ---------------------------------------------*/
/*------------------------------------------- PRIVATE TYPEDEFS -------------------------------------------------------*/
/*------------------------------------------- STATIC VARIABLES -------------------------------------------------------*/
/*------------------------------------------- GLOBAL VARIABLES -------------------------------------------------------*/
/*------------------------------------------- STATIC FUNCTION PROTOTYPES ---------------------------------------------*/
/*------------------------------------------- STATIC FUNCTIONS -------------------------------------------------------*/
/*------------------------------------------- GLOBAL FUNCTIONS -------------------------------------------------------*/

MA12040P_RESULT ma12040p_setVolume(
    BM_TWI *twi,
    float volDb)
{
    uint8_t volInt, regValInt, regValFrac;
    float volFrac;
    MA12040P_RESULT ret = MA12040P_SUCCESS;

    // check NULL pointers
    if (NULL == twi)
    {
        return MA12040P_ERROR;
    }

    // check limits
    volDb = (volDb > MA12040P_VOL_MAX) ? MA12040P_VOL_MAX : \
            (volDb < MA12040P_VOL_MIN) ? MA12040P_VOL_MIN : volDb;

    // round volume to nearest 0.25dB level
    volDb = floor(volDb * 4U) / 4U;

    // get integer and fractional parts
    volInt = (uint8_t)volDb;
    volFrac = (float)volInt - volDb;

    // calc integer register value
    regValInt = (uint8_t)MA12040P_VOL_MAX - volInt;

    // calculate fractional register value
    if (volFrac < 0.0F)
    {
        volFrac += 1.0F;
    }
    regValFrac = (uint8_t)(volFrac * 4U);

    // write values to registers
    ret |= ma12040p_writeReg(twi, MA12040P_VOL_DB_MASTER_ADDR, regValInt, MA12040P_VOL_DB_MASTER_BMASK,
                             MA12040P_VOL_DB_MASTER_BPOS);
    ret |= ma12040p_writeReg(twi, MA12040P_VOL_LSB_MASTER_ADDR, regValFrac, MA12040P_VOL_LSB_MASTER_BMASK, 
                             MA12040P_VOL_LSB_MASTER_BPOS);

    return ret;
}

MA12040P_RESULT ma12040p_setVlaEnable(
    BM_TWI *twi,
    ma12040p_vlaEnable_t enDi)
{
    // check NULL pointers
    if (NULL == twi)
    {
        return MA12040P_ERROR;
    }

    return ma12040p_writeReg(twi, MA12040P_AUDIO_PROC_ENABLE_ADDR, enDi, MA12040P_AUDIO_PROC_ENABLE_BMASK,
                             MA12040P_AUDIO_PROC_ENABLE_BPOS);
}

MA12040P_RESULT ma12040p_writeReg(
    BM_TWI *twi,
    uint8_t regAddr,
    uint8_t regValue,
    uint8_t regBitMask,
    uint8_t regBitPos)
{
    uint8_t rdData;
    uint8_t wrData[2] = {regAddr, 0U};
    uint8_t *pWrVal = &wrData[1];

    // check NULL pointers
    if (NULL == twi)
    {
        return MA12040P_ERROR;
    }

    // read the register addres
    ma12040p_readReg(twi, regAddr, pWrVal);

    // clear the bits
    *pWrVal &= ~(regBitMask << regBitPos);

    // write the new data
    *pWrVal |= regValue << regBitPos;

    // write param data
    if (twi_write_block_r(twi, wrData, 2U, false) != TWI_SIMPLE_SUCCESS)
    {
        return MA12040P_ERROR;
    }

    // read back the value to check it has been written correctly
    if (ma12040p_readReg(twi, regAddr, &rdData) != MA12040P_SUCCESS)
    {
        return MA12040P_ERROR;
    }
    else
    {
        if (rdData != *pWrVal)
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
    // check NULL pointers
    if ((NULL == twi) || (NULL == regValue))
    {
        return MA12040P_ERROR;
    }

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
