/***********************************************************************************************************************
 * 
 * @file    mcAmp.c
 *
 * @brief   Multichannel Amplifier API's
 *  
 * @par
 * @author  G. Howell
 * 
 **********************************************************************************************************************/

/*------------------------------------------- INCLUDES ---------------------------------------------------------------*/

#include "mcamp.h"

#include "ma12040p_driver.h"

#include <sruSC589.h>
#include "drivers/bm_twi_driver/bm_twi.h"
#include "drivers/bm_event_logging_driver/bm_event_logging.h"

/*------------------------------------------- EXTERN VARIABLES -------------------------------------------------------*/
/*------------------------------------------- PRIVATE MACROS AND DEFINES ---------------------------------------------*/

#define MCAMP_N_BUSSES							(2U)
#define MCAMP_N_AMPS_ON_BUS_MAX					(4U)

// initialisation parameters
#define MCAMP_N_INIT_PARAMS						(1U)
#define MCAMP_INIT_VOLUME						(0x30U) // -24 dBFS

/*------------------------------------------- PRIVATE TYPEDEFS -------------------------------------------------------*/

typedef struct mcAmp_initParams_t
{
	char name[20];
	uint8_t regAddr; 
	uint8_t regValue;
	uint8_t regBitMask,
	uint8_t regBitPos
} mcAmp_initParams_t;

/*------------------------------------------- STATIC VARIABLES -------------------------------------------------------*/
/*------------------------------------------- GLOBAL VARIABLES -------------------------------------------------------*/

BM_TWI mcAmpTwiH;

// Used to tell which amps are connected, and to which i2c mux bus, by storing the corresponding bit value.
// For example, if there are a maximum of 4 devices per bus, and 2 busses, and all devices are connected on both
// busses, then the value will be 0xFF. However, if only the first two devices are connected on bus 1 and the last
// two devices connected on bus 2, then the value will be 0xC3.
uint16_t ma12040pDevMap;

mcAmp_initParams_t initParams[MCAMP_N_INIT_PARAMS] = {
	{"master volume", MA12040P_VOL_DB_MST_ADDR, MCAMP_INIT_VOLUME, MA12040P_VOL_DB_MST_BMASK, MA12040P_VOL_DB_MST_BPOS}
};

/*------------------------------------------- STATIC FUNCTION PROTOTYPES ---------------------------------------------*/

static void i2cInit(
	void);

static void setI2cMuxBus(
	uint8_t busNum);
	
static void detectAmps(
	void);

static void setInitValues(
	void);

/*------------------------------------------- STATIC FUNCTIONS -------------------------------------------------------*/

/* i2c initialisation */
static void i2cInit(void)
{
	char msg[128] = {0};

	// initialise i2c
	if (twi_initialize(&mcAmpTwiH, MCAMP_I2C_MUX_DEV_ADDR, TWI_TYPICAL_SCLK0_FREQ, TWI2) != TWI_SIMPLE_SUCCESS)
	{
		log_event(EVENT_WARN, "McAmp: error initialising i2c");
	}

	// set baud rate
	twi_set_clock(&mcAmpTwiH, MCAMP_I2C_SPEED);

	log_event(EVENT_INFO, "McAmp: I2C initialised");

	sprintf(msg, "McAmp: I2C Config, Bus: %d, Addr: 0x%.2x, Speed: %.3f KHz",
			TWI2, MCAMP_I2C_MUX_DEV_ADDR, (double)MCAMP_I2C_SPEED / 1000.0);
	log_event(EVENT_INFO, msg);
}

/* set the i2c mux bus number */
static void setI2cMuxBus(
	uint8_t busNum)
{
	uint8_t busNumRegVal, regVal;
	
	busNumRegVal = 1U << busNum;

	// write bus number
	if (twi_write_block_r(&mcAmpTwiH, &busNumRegVal, 1, false) != TWI_SIMPLE_SUCCESS)
	{
		log_event(EVENT_WARN, "McAmp: error writing bus number to i2c mux");
	}

	// readback bus number of check against sent value
	if (twi_read(&mcAmpTwiH, &regVal) != TWI_SIMPLE_SUCCESS)
	{
		log_event(EVENT_WARN, "McAmp: error reading the i2c mux bus");
	}
	else
	{
		if (regVal != busNumRegVal)
		{
			log_event(EVENT_WARN, "McAmp: error setting the i2c mux bus");
		}
	}
}

/* detect all connected amps */
static void detectAmps(void)
{
	uint8_t i, j, regVal;
	char msg[128] = {0U};

	for (i = 0U; i < MCAMP_N_BUSSES; i++)
	{
		// set i2c mux bus number
		setI2cMuxBus(i);

		// go through each possible ma12040p device address and read the master volume register, if the read returns
		// the correct default value (0x18) then the device is connected
		for (j = 0U; j < MCAMP_N_AMPS_ON_BUS_MAX; j++)
		{
			// set i2c address to the current amp
			twi_set_temporary_address(&mcAmpTwiH, MA12040P_DEVADDR_1 + j);

			// read the value of the master volume register
			if (ma12040p_readReg(&mcAmpTwiH, MA12040P_VOL_DB_MST_ADDR, &regVal) != MA12040P_SUCCESS)
			{
				log_event(EVENT_WARN, "McAmp: error reading register");
			}

			if (regVal == MA12040P_VOL_DB_MST_DVAL)
			{
				// add device to device map
				ma12040pDevMap |= 1U << (j + i * MCAMP_N_AMPS_ON_BUS_MAX);

				sprintf(msg, "McAmp: MA12040P amp found. I2C Mux Bus: %d; Device Address: 0x%.2x", i, MA12040P_DEVADDR_1 + j);
				log_event(EVENT_INFO, msg);
			}
			else
			{
				sprintf(msg, "McAmp: MA12040P amp not found. I2C Mux Bus: %d; Device Address: 0x%.2x", i, MA12040P_DEVADDR_1 + j);
				log_event(EVENT_WARN, msg);
			}
		}

		// restore device address back to the i2c mux
		twi_restore_address(&mcAmpTwiH);
	}

	// reset the i2c mux bus to default (0)
	setI2cMuxBus(0U);

	if (ma12040pDevMap == 0U)
	{
		log_event(EVENT_WARN, "McAmp: No amps connected.");
	}
}

static void setInitValues(void)
{
	uint32_t i, j, k;
	char msg[128] = {0};

	for (i = 0; i < MCAMP_N_BUSSES; i++)
	{
		setI2cMuxBus(i);

		for (j = 0; j < MCAMP_N_AMPS_ON_BUS_MAX; j++)
		{
			if (ma12040pDevMap & (1U << (j + i * MCAMP_N_AMPS_ON_BUS_MAX)))
			{
				twi_set_temporary_address(&mcAmpTwiH, MA12040P_DEVADDR_1 + j);

				for (k = 0; k < MCAMP_N_INIT_PARAMS; k++)
				{
					if (ma12040p_writeReg(&mcAmpTwiH, initParams[k].regAddr, initParams[k].regValue, 
					   					  initParams[k].regBitMask, initParams[k].regBitPos) != MA12040P_SUCCESS)
					{
						sprintf(msg, "McAmp: error writing '%s' value 0x%x", initParams[k].name, 
							    initParams[k].regValue);
						log_event(EVENT_WARN, msg);
					}
					else
					{
						sprintf(msg, "McAmp: written '%s' value 0x%x", initParams[k].name, initParams[k].regValue);
						log_event(EVENT_INFO, msg);
					}
				}
			}
		}

		twi_restore_address(&mcAmpTwiH);
	}
}

/*------------------------------------------- GLOBAL FUNCTIONS -------------------------------------------------------*/

/* initialise */
void mcAmp_init(
	void)
{
	// i2c initialisation
	i2cInit();

	// check any amps connected
	detectAmps();

	// set initial value
	setInitValues();
}

/* enable */
void mcAmp_enable(
	void)
{
	SRU2(LOW, DAI1_PB14_I);

	log_event(EVENT_INFO, "McAmp: enabled amps");
}
