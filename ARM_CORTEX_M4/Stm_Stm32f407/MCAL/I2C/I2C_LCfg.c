/*
 * I2C_LCfg.c
 *
 *  Created on: May 16, 2020
 *      Author: Reem
 */

#include "I2C_LCfg.h"

strI2C_cfg_t I2C_Cfg[NUM_OF_I2C_OBJECTS] ={
{.I2C_ChannelNo = I2C_1,
 .GeneralCallEN = GC_ENABLE,
 .ClkStretchingEN = CLK_STRETCH_ENABLE,
 .PEC_EN = PEC_ENABLE,
 .ACK_EN = ACK_ENABLE,
 .BufferInterruptEN = INT_BUFFER_ENABLE,
 .EventInterruptEN = INT_EVENT_ENABLE,
 .ErrorInterruptEN = INT_ERR_ENABLE,
 .InterfaceAddMode = _7_BIT_ADDRESS,
 .SlaveAddress7BitsOAR1 = 44,
 .SlaveAddress7BitsOAR2 = 45,
 .SlaveAddress10Bit_B0 = 0,
 .SlaveAddress10Bit_B8_B9 = 0,
 .DualModeEN = DUAL_ADDRESS_ENABLE,
 .MasterModeSelection = SM_ENABLE,
 .Duty = DUTY_16TLOW_9THIGH_ENABLE,
 .CLK = 200,
 .AnalogNoiseFilterEN = AN_DISABLE,
 .DigitalNoiseFilterVal = 5}
};
