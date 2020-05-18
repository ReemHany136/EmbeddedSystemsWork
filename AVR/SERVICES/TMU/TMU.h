/*
 * TMU.h
 *
 * Created: 2/24/2020 11:47:11 AM
 *  Author: Reem
 */ 
#ifndef TMU_H_
#define TMU_H_
#include "std_types.h"
#include "Timer.h"

/****************************************************/
/*              Defines                             */
/****************************************************/
//TMU Channels
#define		TMU_TIMER_CH0					0
#define		TMU_TIMER_CH1					1
#define		TMU_TIMER_CH2					2

//TMU Modes
#define     ONE_SHOT                        0
#define     PERIODIC                        1

//TMU Resolution
#define TMU_RESOLUTION                      250

//TMU Buffer size
#define TMU_TASK_BUFFER_SIZE                100

//ERROR Value Of the Module 
#define TMU_ERROR                          -700

       

/************************************************************************/
/*			  Structures Definitions		                            */
/************************************************************************/
typedef struct TMU_Cfg{
	uint8_t TIM_Ch;
	uint8_t Resolutions;
}TMU_Cfg;
typedef struct TMU_TASK_Cfg{
	uint16_t Task_id;
	void (*TMU_Cbk_ptr)(void);
	uint8_t TMU_MODE;
	uint32_t Delay_TimeMs;
	uint32_t Delay_Counter;
	
}TMU_TASK_Cfg;

/************************************************************************/
/*			  Functions Prototypes		                                */
/************************************************************************/

/**
 * Input: Pointer to a structure contains the information needed to initialize the TMU. 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the timer.
 * 							
 */
extern ERROR_STATUS TMU_Init(TMU_Cfg* TMU_cfg);

/**
 * Input:  
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: this function De-initializes the module
 * 							
 */
extern ERROR_STATUS TMU_DeInit(void);

/**
 * Input: 
 * 	task: Pointer to a structure contains the information needed to create the TMU task.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function act as the TMU creator as it starts the initialized timer , 
 *              creates the task and save it in the TMU task buffer.
 * 							
 */
extern ERROR_STATUS TMU_Start_Timer(TMU_TASK_Cfg* task);

/**
 * Input: 
 * 	taskID: the id of a specific task.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function removes the task with the given id.
 * 							
 */
extern ERROR_STATUS TMU_Stop_Timer(uint8_t taskID);

/**
 * Input: 
 * 	taskID: the id of a specific task.
 * Output:
 * In/Out:			
 * Return: 			
 * Description: This function act as the TMU Manager that manages the TMU task buffer and calls the TMU Consumers for each task
 *				 when the time is finished of this task
 * 							
 */
extern void TMU_Dispatcher(void);


#endif /* TMU_H_ */