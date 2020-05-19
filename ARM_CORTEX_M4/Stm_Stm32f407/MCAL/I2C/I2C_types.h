/*
 * I2C_types.h
 *
 *  Created on: May 16, 2020
 *      Author: Reem
 */

#ifndef MCAL_I2C_TYPES_H_
#define MCAL_I2C_TYPES_H_
#include "std_types.h"
#include "PerRegs.h"

/*I2C Channel*/
typedef uint8_t I2CChannel_t;
#define I2C_1     1
#define I2C_2     2
#define I2C_3     3

/*PEC Enable*/
typedef uint8_t ENPEC_t;
#define PEC_ENABLE     1
#define PEC_DISABLE    0

/*ACK Enable*/
typedef uint8_t ENACK_t;
#define ACK_ENABLE     1
#define ACK_DISABLE    0


/*CLK Stretching Enable*/
typedef uint8_t ENClkStretching_t;
#define CLK_STRETCH_ENABLE     0
#define CLK_STRETCH_DISABLE    1

/*General Call Enable*/
typedef uint8_t ENGeneralCall_t;
#define GC_ENABLE     1
#define GC_DISABLE    0


/*Buffer interrupt Enable*/
typedef uint8_t ENBufferINT_t;
#define INT_BUFFER_ENABLE     1
#define INT_BUFFER_DISABLE    0

/*Event interrupt Enable*/
typedef uint8_t ENEventINT_t;
#define INT_EVENT_ENABLE     1
#define INT_EVENT_DISABLE    0

/*Error interrupt Enable*/
typedef uint8_t ENErrorINT_t;
#define INT_ERR_ENABLE     1
#define INT_ERR_DISABLE    0

/*Interface Address Mode*/
typedef uint8_t ADDMODE_t;
#define _7_BIT_ADDRESS     0
#define _10_BIT_ADDRESS    1

/*Slave Address */
typedef uint8_t SLAVEAddress_t;

/*Dual Address Mode*/
typedef uint8_t DualMODE_t;
#define DUAL_ADDRESS_ENABLE     1
#define DUAL_ADDRESS_DISABLE    0

/*Master Mode*/
typedef uint8_t FS_t;
#define SM_ENABLE     0
#define FM_ENABLE     1

/*Duty Mode*/
typedef uint8_t DutyVAL_t;
#define DUTY_2TLOW_1THIGH_ENABLE     0
#define DUTY_16TLOW_9THIGH_ENABLE    1

/*Analog Noise Mode*/
typedef uint8_t AnalogNoise_t;
#define AN_ENABLE     0
#define AN_DISABLE    1

typedef struct strI2C_cfg_t
{
	uint8_t I2C_ChannelNo;
	ENGeneralCall_t GeneralCallEN;
	ENClkStretching_t ClkStretchingEN;
	ENPEC_t PEC_EN;
	ENACK_t ACK_EN;

	ENBufferINT_t BufferInterruptEN;
	ENEventINT_t EventInterruptEN;
	ENErrorINT_t ErrorInterruptEN;


	ADDMODE_t InterfaceAddMode;
	SLAVEAddress_t SlaveAddress7BitsOAR1;
	SLAVEAddress_t SlaveAddress7BitsOAR2;
	SLAVEAddress_t SlaveAddress10Bit_B0;
	SLAVEAddress_t SlaveAddress10Bit_B8_B9;
	DualMODE_t DualModeEN;

	FS_t MasterModeSelection;
	DutyVAL_t Duty;
	uint8_t CLK;

	AnalogNoise_t AnalogNoiseFilterEN;

	uint8_t DigitalNoiseFilterVal;

}strI2C_cfg_t;


#endif /* MCAL_I2C_TYPES_H_ */
