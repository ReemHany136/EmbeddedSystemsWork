/*
 * SOS_PCfg.h
 *
 * Created: 3/10/2020 11:11:43 PM
 *  Author: Reem
 */ 


#ifndef SOS_PCFG_H_
#define SOS_PCFG_H_

#include "std_types.h"

//SOS Channels
#define		SOS_TIMER_CH0					0
#define		SOS_TIMER_CH1					1
#define		SOS_TIMER_CH2					2

//SOS Resolution
#define SOS_RESOLUTION                      250

typedef struct SOS_Cfg{
	uint8_t TIM_Ch;
	uint8_t Resolutions;
}SOS_Cfg;

extern SOS_Cfg SOS_cfg;



#endif /* SOS_PCFG_H_ */