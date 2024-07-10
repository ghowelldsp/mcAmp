/**
 * @file    ma12040p.h
 *
 * @brief   Description
 *
 * @par
 * Created On: 02/07/2022
 * Created By: G. Howell
 */

#ifndef __MA12040P_H__
#define __MA12040P_H__

#ifdef __cplusplus
extern "C" {
#endif

/*------------------- INCLUDES -----------------------------------*/

#include <stdint.h>

/*------------------- MACROS AND DEFINES -------------------------*/

/* 
	MA12040P I2C device addresses
*/

#define MA12040P_DEVADDR_1		((uint8_t)0x20)	// 7-bit device address
#define MA12040P_DEVADDR_2		((uint8_t)0x21)	// 7-bit device address
#define MA12040P_DEVADDR_3		((uint8_t)0x22)	// 7-bit device address
#define MA12040P_DEVADDR_4		((uint8_t)0x23)	// 7-bit device address

/* 
	power mode control registers
*/

//// power mode control
//#define PMC 				((uint8_t)0x00) // manualPM, PM_man
//
//// thresholds for power modes
//#define PM_1_2				((uint8_t)0x01) // Mthr_1to2
//#define PM_2_1				((uint8_t)0x02) // Mthr_2to1
//#define PM_2_3				((uint8_t)0x03) // Mthr_2to3
//#define PM_3_2				((uint8_t)0x04) // Mthr_3to2
//
//// soft-clipping and over- current protection latching
//#define SCLIP_OCPROC		((uint8_t)0x0A) // lf_clamp_en, ocp_latch_en
//
//// select power mode profile setting
//#define PM_PROF				((uint8_t)0x1D) // PMprofile
//
//// power mode profile configuration
//#define PM_PROF_CONFIG		((uint8_t)0x1E) // PM3_man, PM2_man, PM1_man
//
///*
//	power monitor bit values
//*/
//
//#define PM_MAN_PM			6
//#define PM_MAN				4
//#define PM_MAN_BITS			((uint8_t)0x03)
//#define PM_CLIP				7
//#define PM_OCP				1
//#define PM_PROF_BITS 	 	((uint8_t)0x07)
//#define PM_PROF_CFG_1 	 	0
//#define PM_PROF_CFG_2 	 	2
//#define PM_PROF_CFG_3 	 	4
//#define PM_PROF_CFG_BITS 	((uint8_t)0x03)
//
///*
//	audio protection control registers
//*/
//
//// over-current protection latch clear
//#define OCP_LATCH_CLR		((uint8_t)0x20) // ocp_latch_clear
//
//// audio in mode
//#define AUD_IN_MODE			((uint8_t)0x25) // audio_in_mode
//
//// dc protection
//#define DC_PROT				((uint8_t)0x26) // Eh_dcShdn
//
//// audio in mode overwrite
//#define AUD_IN_MODE_OW		((uint8_t)0x27) // audio_in_mode_ext
//
///*
//	audio protection bit values
//*/
//#define	AP_AI_MODE			5
//#define	AP_AI_MODE_BITS		((uint8_t)0x03)
//#define	AP_AI_MODE_OVER		5
//#define AP_DC_PROC			2
//#define AP_OCP_CLR			7
//
///*
//	i2s config control registers
//*/
//
//// pcm word format
//#define I2S_PCM_FMT			((uint8_t)0x35) // i2s_format
//
//// other i2c config paramters
//#define I2S_CNF_1			((uint8_t)0x36) // i2s_rightfirst, i2s_framesize, i2s_order, i2s_ws_pol, i2s_sck_pol
//
///*
//	i2s config register bit values
//*/
//
//#define I2S_PCM_BITS		((uint8_t)0x07)
//#define I2S_RFIRST			5
//#define I2S_FSIZE			3
//#define I2S_FSIZE_BITS		((uint8_t)0x03)
//#define I2S_ORDER			2
//#define I2S_WS_POL			1
//#define I2S_SCK_POL			0

/*
 * VLA Control Registers and Settings
 */

// audio_proc_release
#define MA12040P_AUDIO_PROC_RELEASE_ADDR			((uint8_t)0x35)		// address
#define MA12040P_AUDIO_PROC_RELEASE_BMASK			((uint8_t)0x03) 	// bit mask
#define MA12040P_AUDIO_PROC_RELEASE_BPOS			((uint8_t)0x06) 	// bit position

// audio_proc_attack
#define MA12040P_AUDIO_PROC_ATTACK_ADDR				(MA12040P_AUDIO_PROC_RELEASE_ADDR)	// address
#define MA12040P_AUDIO_PROC_ATTACK_BMASK			((uint8_t)0x03) 	// bit mask
#define MA12040P_AUDIO_PROC_ATTACK_BPOS				((uint8_t)0x04) 	// bit position

// audio_proc_enable
#define MA12040P_AUDIO_PROC_ENABLE_ADDR				(MA12040P_AUDIO_PROC_RELEASE_ADDR)	// address
#define MA12040P_AUDIO_PROC_ENABLE_BMASK			((uint8_t)0x01) 	// bit mask
#define MA12040P_AUDIO_PROC_ENABLE_BPOS				((uint8_t)0x03) 	// bit position

#define MA12040P_VOL_MAX							(24.0F)
#define MA12040P_VOL_MIN							(-144.0F)

// vol_db_master
#define MA12040P_VOL_DB_MASTER_ADDR					((uint8_t)0x40) 	// address
#define MA12040P_VOL_DB_MASTER_BMASK				((uint8_t)0xFF) 	// bit mask
#define MA12040P_VOL_DB_MASTER_BPOS					((uint8_t)0x00) 	// bit position
#define MA12040P_VOL_DB_MASTER_DVAL					((uint8_t)0x18) 	// default value

// vol_lsb_master
#define MA12040P_VOL_LSB_MASTER_ADDR				((uint8_t)0x41)		// address
#define MA12040P_VOL_LSB_MASTER_BMASK				((uint8_t)0x03) 	// bit mask
#define MA12040P_VOL_LSB_MASTER_BPOS				((uint8_t)0x00) 	// bit position

/*
 * VLA Settings
 */

typedef enum
{
	MA12040P_AR_TIME_SLOW,
	MA12040P_AR_TIME_NORMAL,
	MA12040P_AR_TIME_FAST,
} ma12040p_arTimes_t

//// channel volumes db
//#define VOL_DB_CH0			((uint8_t)0x42) // vol_db_ch0
//#define VOL_DB_CH1			((uint8_t)0x43) // vol_db_ch1
//#define VOL_DB_CH2			((uint8_t)0x44) // vol_db_ch2
//#define VOL_DB_CH3			((uint8_t)0x45) // vol_db_ch3
//
//// channel volumes lsb
//#define VOL_LSB_CH0_3		((uint8_t)0x46) // vol_lsb_ch0, vol_lsb_ch1, vol_lsb_ch2, vol_lsb_ch3
//
//// channel threshold db
//#define VOL_TH_DB_CH0		((uint8_t)0x47) // thr_db_ch0
//#define VOL_TH_DB_CH1		((uint8_t)0x48) // thr_db_ch1
//#define VOL_TH_DB_CH2		((uint8_t)0x49) // thr_db_ch2
//#define VOL_TH_DB_CH3		((uint8_t)0x4A) // thr_db_ch3
//
//// channel threshold lsb
//#define TH_LSB_CH0_3		((uint8_t)0x4B) // thr_lsb_ch0, thr_lsb_ch1, thr_lsb_ch2, thr_lsb_ch3
//
//// limiter + clipping monitor
//#define AUDP_LC_MON			((uint8_t)0x7E) // audio_proc_limiter_mon, audio_proc_clip_mon
//
///*
//	volume registers bit values
//*/
//#define VOL_ATT_SHIFT		6
//#define VOL_ATT_BIT			((uint8_t)0x03)
//#define VOL_REL_SHIFT		4
//#define VOL_REL_BIT			((uint8_t)0x03)
//#define VOL_BPASS_SHIFT		3
//#define VOL_MUTE_SHIFT		7
//#define VOL_BYLIM_SHIFT		4
//#define VOL_LSB_BITS		((uint8_t)0x03)
//#define VOL_LSB_CH0_SHIFT	0
//#define VOL_LSB_CH1_SHIFT	2
//#define VOL_LSB_CH2_SHIFT	4
//#define VOL_LSB_CH3_SHIFT	6
//#define VOL_LIM_ACT_SHIFT	4
//#define VOL_CLIP_SHIFT		0
//
///*
//	Channel Monitors
//*/
//
//// monitor register channel 0 (freq + power)
//#define MON_REG_FP_CH0		((uint8_t)0x60) // dcu_mon0.freqMode, dcu_mon0.PM_mon
//
//// monitor register channel 0
//#define MON_REG_CH0			((uint8_t)0x61) // dcu_mon0.mute, dcu_mon0.vdd_ok, dcu_mon0.pvdd_ok, dcu_mon0.Vcfly2_ok, dcu_mon0.Vcfly1_ok, OCP Monitor channel 0
//
//// monitor register channel 0 (mod index)
//#define MON_REG_MOD_CH0		((uint8_t)0x62) // dcu_mon0.M_mon
//
//// monitor register channel 1 (freq + power)
//#define MON_REG_FP_CH1		((uint8_t)0x64) // dcu_mon1.freqMode, dcu_mon1.PM_mon
//
//// monitor register channel 1
//#define MON_REG_CH1			((uint8_t)0x65) // dcu_mon1.mute, dcu_mon1.vdd_ok, dcu_mon1.pvdd_ok, dcu_mon1.Vcfly2_ok, dcu_mon1.Vcfly1_ok, OCP Monitor channel 1
//
//// monitor register channel 1 (mod index)
//#define MON_REG_MOD_CH1		((uint8_t)0x66) // dcu_mon1.M_mon
//
///*
//	error registers
//*/
//
//// error accumlation reg
//#define ERR_ACC				((uint8_t)0x6D) // error_acc
//
//// error handler clear
//#define ERR_H_CLR			((uint8_t)0x2D) // eh_clear
//
//// monitor output config
//#define MON_OUT_CONFIG		((uint8_t)0x75) // msel_mon
//
//// live error reg
//#define ERR_LIVE			((uint8_t)0x7C) // error
//
///*
//	error register bit values
//*/
//#define ERR_H_CLR_BIT		2
//
///*
//	monitor register bit values
//*/
//
//// frequency and power modes
//#define MON_FREQ_M 			4
//#define MON_FREQ_M_BITS 	((uint8_t)0x07)
//#define MON_POW_M_BITS 		((uint8_t)0x03)
//
//#define MON_OCP_MON			0
//#define MON_FLY_1 			1
//#define MON_FLY_2 			2
//#define MON_PVDD_OK 		3
//#define MON_VDD_OK 			4
//#define MON_MUTE 			5
//
//// MSEL bits
//#define MON_MSEL_BITS		((uint8_t)0x7)
//// THESE NEED TO BE DOUBLE CHECKED!!! - no avaliable info
//#define MON_BTL				1
//#define MON_SE 				2
//#define MON_BTL_SE 			3
//#define MON_PBTL 			4

/*------------------- TYPEDEFS -----------------------------------*/
/*------------------- EXPORTED VARIABLES -------------------------*/
/*------------------- GLOBAL FUNCTION PROTOTYPES -----------------*/

#ifdef __cplusplus
}
#endif

#endif /* __MA12040P_H__ */
