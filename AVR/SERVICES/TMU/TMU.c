/*
 * TMU.c
 *
 * Created: 2/24/2020 11:47:28 AM
 *  Author: Reem
 */
#include "TMU.h"

/*******Static Global Variables*********/
static TMU_Cfg gstr_TMUcfg;
static Timer_cfg_s gstr_TimerCfg;
static TMU_TASK_Cfg *gstr_TasksBuffer[TMU_TASK_BUFFER_SIZE];

static uint8_t gu8_InitFlag=FALSE,
               gu8_StartedFlag = FALSE,
			   gu8_ISR_Flag=FALSE;


/**********Static Prototypes************/

static ERROR_STATUS TMU_RemoveTask(uint16_t TaskID);
static void TMU_CheckFlag(void);

/*******Function Implementation*********/

/**
 * Input:
 * Output:
 * In/Out:
 * Return:
 * Description: this function used to act as a callback function that is called by the ISR in timer module to inform the TMU Dispatcher that the ISR came
 *
 */
static void TMU_CheckFlag(void){
	gu8_ISR_Flag = TRUE;
}

/**
 * Input: Pointer to a structure contains the information needed to initialize the TMU.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the timer.
 *
 */
ERROR_STATUS TMU_Init(TMU_Cfg* TMU_cfg){
	//Define locals
	ERROR_STATUS aES_errorStatus;

	/*Check if the module is initialized or not*/

	//In case the module is uninitialized
	if(gu8_InitFlag == FALSE){
		//ERROR assignment
		aES_errorStatus = E_OK;

		/*Check whether the parameter is a null pointer or not*/

		//In case the pointer is not equal to null
		if (TMU_cfg != NULL)
		{
			//ERROR assignment
			aES_errorStatus = E_OK;

			//Check on timer channel
			switch(TMU_cfg->TIM_Ch){
				case TMU_TIMER_CH0:
				//Timer initialization
				gstr_TimerCfg.Timer_CH_NO = TMU_TIMER_CH0;
				gstr_TimerCfg.Timer_Polling_Or_Interrupt = TIMER_INTERRUPT_MODE;

				//Resolution >> prescaler 64
				gstr_TimerCfg.Timer_Prescaler = TIMER_PRESCALER_64;
				gstr_TimerCfg.Timer_Cbk_ptr= TMU_CheckFlag;

				//ERROR assignment
				aES_errorStatus = Timer_Init(&gstr_TimerCfg);

				gstr_TMUcfg.TIM_Ch = TMU_TIMER_CH0;
				gstr_TMUcfg.Resolutions = TMU_RESOLUTION;
				break;

				case TMU_TIMER_CH1:
				//Timer initialization
				gstr_TimerCfg.Timer_CH_NO = TMU_TIMER_CH1;
				gstr_TimerCfg.Timer_Polling_Or_Interrupt = TIMER_INTERRUPT_MODE;

				//Resolution >> prescaler 64
				gstr_TimerCfg.Timer_Prescaler = TIMER_PRESCALER_64;
				gstr_TimerCfg.Timer_Cbk_ptr= TMU_CheckFlag;

				//ERROR assignment
				aES_errorStatus = Timer_Init(&gstr_TimerCfg);

				gstr_TMUcfg.TIM_Ch = TMU_TIMER_CH1;
				gstr_TMUcfg.Resolutions = TMU_RESOLUTION;
				/*//ERROR assignment
				aES_errorStatus = (E_INVALID_PARAMETER + TMU_ERROR);*/
				break;

				case TMU_TIMER_CH2:

				//Timer initialization
				gstr_TimerCfg.Timer_CH_NO = TMU_TIMER_CH2;
				gstr_TimerCfg.Timer_Polling_Or_Interrupt = TIMER_INTERRUPT_MODE;

				//Resolution >> prescaler 64
				gstr_TimerCfg.Timer_Prescaler = TIMER_PRESCALER_64;
				gstr_TimerCfg.Timer_Cbk_ptr= TMU_CheckFlag;

				//ERROR assignment
				aES_errorStatus = Timer_Init(&gstr_TimerCfg);

				gstr_TMUcfg.TIM_Ch = TMU_TIMER_CH2;
				gstr_TMUcfg.Resolutions = TMU_RESOLUTION;

				break;

				default:
				//ERROR assignment
				aES_errorStatus = (E_INVALID_PARAMETER + TMU_ERROR);
				break;

			}

		}
		//In case the pointer is Null
		else{
			//ERROR assignment
			aES_errorStatus = (E_NULL_PTR + TMU_ERROR);
		}
	}
	//In case the module is already initialized
	else{
		//ERROR assignment
		aES_errorStatus = (E_MULTIPLE_INITIALIZION + TMU_ERROR);
	}

	//Check whether the error is OK or not
	if (aES_errorStatus == E_OK)
	{
		//Mark that the module is initialized successfully
		gu8_InitFlag = TRUE;
	}
	else{
		//Mark that the module is initialized unsuccessfully
		gu8_InitFlag = FALSE;
	}

	//Return the error status
	return aES_errorStatus;
}

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: this function De-initializes the module
 *
 */
ERROR_STATUS TMU_DeInit(void){
	//Define locals
	ERROR_STATUS aES_errorStatus;
	uint16_t au16_TaskIterator;


	/*Check whether the module is initialized or not*/

	//The module has already been initialized
	if (gu8_InitFlag == TRUE)
	{
		//ERROR assignment
		aES_errorStatus = E_OK;

		//LOOP on the whole buffer to reset it
		for (au16_TaskIterator = 0; au16_TaskIterator < TMU_TASK_BUFFER_SIZE; au16_TaskIterator++)
			gstr_TasksBuffer[au16_TaskIterator] = NULL;
		switch(gstr_TMUcfg.TIM_Ch){
			case TMU_TIMER_CH0:
			aES_errorStatus = Timer_Stop(TMU_TIMER_CH0);
			break;
			case TMU_TIMER_CH1:
			aES_errorStatus = Timer_Stop(TMU_TIMER_CH1);
			break;
			case TMU_TIMER_CH2:
			aES_errorStatus = Timer_Stop(TMU_TIMER_CH2);
			break;
			default:
			aES_errorStatus =(E_INVALID_PARAMETER + TMU_ERROR);
			break;
		}
		if(aES_errorStatus ==E_OK){
			//MARK that the module is De-initialized
		    gu8_InitFlag = FALSE;
		}

	}
	else{
		//ERROR assignment
		aES_errorStatus = (E_UNINITIALIZED_MODULE + TMU_ERROR);
	}

	//Return the error status
	return aES_errorStatus;
}

/**
 * Input:
 * 	task: Pointer to a structure contains the information needed to create the TMU task.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function act as the TMU creator as it starts the initialized timer , creates the task and save it in the TMU task buffer.
 *
 */
ERROR_STATUS TMU_Start_Timer(TMU_TASK_Cfg* task){

	//Define locals
	uint16_t au16_TaskIterator;
	uint8_t  au8_CreatedFlag=FALSE;
	ERROR_STATUS aES_errorStatus;

	/*Check whether the module is initialized or not*/

	//In case the module is already initialized
	if(gu8_InitFlag == TRUE){

		//ERROR assignment
		aES_errorStatus = E_OK;

		/*Check whether the pointer is null or not*/

		//In case the pointer not equal to NULL
		if(task != NULL){
			//ERROR assignment
			aES_errorStatus = E_OK;

			/*Task parameters checking*/

			task->Delay_Counter = task->Delay_TimeMs;
			for (au16_TaskIterator = 0; au16_TaskIterator < TMU_TASK_BUFFER_SIZE; au16_TaskIterator++)
			{
			    if(gstr_TasksBuffer[au16_TaskIterator] != NULL){
                    if (task->Task_id == gstr_TasksBuffer[au16_TaskIterator]->Task_id)
                    {
                        aES_errorStatus = (E_INVALID_PARAMETER + TMU_ERROR);
                        break;
                    }
			    }

			}
			if (aES_errorStatus == E_OK)
			{


				switch(task->TMU_MODE){
				case PERIODIC:
				//ERROR assignment
				aES_errorStatus = E_OK;
				break;

				case ONE_SHOT:
				//ERROR assignment
				aES_errorStatus = E_OK;
				break;

				default:
				//ERROR assignment
				aES_errorStatus = (E_INVALID_PARAMETER + TMU_ERROR);
				break;
			}

			/*ERROR Checking to decide whether to continue the next step or not*/


			//In case error is OK
			if (aES_errorStatus == E_OK)
			{
				/*Check whether the pointer to function is NULL or not*/

				//In case the pointer is NULL
				if(task->TMU_Cbk_ptr == NULL){
					//ERROR assignment
					aES_errorStatus = (E_NULL_PTR + TMU_ERROR);
				}
				//In case the the pointer is not equal to NULL
				else{

					//ERROR assignment
					aES_errorStatus = E_OK;

					/*Loop on the whole Buffer, Get the first empty place and save the task in this place in the buffer*/
					for(au16_TaskIterator = 0; au16_TaskIterator < TMU_TASK_BUFFER_SIZE ;au16_TaskIterator++){
						if(gstr_TasksBuffer[au16_TaskIterator] == NULL){
							au8_CreatedFlag =TRUE;
							gstr_TasksBuffer[au16_TaskIterator] = task;
							break;
						}
					}

					/*Check on timer channel*/
					switch(gstr_TMUcfg.TIM_Ch){
						//In case using Timer 0
						case TMU_TIMER_CH0:
						if(au8_CreatedFlag == FALSE){
							//ERROR assignment
							aES_errorStatus = (E_FULL_BUFFER + TMU_ERROR);
						}
						else{


							/*Check whether the timer is started already or not*/

							//In case the timer is not started
							if(gu8_StartedFlag == FALSE){
								//Start the timer
								aES_errorStatus = Timer_Start(TMU_TIMER_CH0,(_8BIT_RESOLUTION_-gstr_TMUcfg.Resolutions));
								//Mark that the timer is started successfully
								gu8_StartedFlag = TRUE;
							}
							//In case the timer is started
							else{
								//ERROR assignment
								aES_errorStatus = E_OK;
							}
						}
						break;

						//In case using Timer 1
						case TMU_TIMER_CH1:
						if(au8_CreatedFlag == FALSE){
							//ERROR assignment
							aES_errorStatus = (E_FULL_BUFFER + TMU_ERROR);
						}
						else{


							/*Check whether the timer is started already or not*/

							//In case the timer is not started
							if(gu8_StartedFlag == FALSE){
								//Start the timer
								aES_errorStatus = Timer_Start(TMU_TIMER_CH1,(_16BIT_RESOLUTION_-gstr_TMUcfg.Resolutions));
								//Mark that the timer is started successfully
								gu8_StartedFlag = TRUE;
							}
							//In case the timer is started
							else{
								//ERROR assignment
								aES_errorStatus = E_OK;
							}
						}
						/*//ERROR assignment
						aES_errorStatus = (E_INVALID_PARAMETER + TMU_ERROR);*/
						break;

						//In case using Timer 2
						case TMU_TIMER_CH2:
						if(au8_CreatedFlag == FALSE){
							//ERROR assignment
							aES_errorStatus = (E_FULL_BUFFER + TMU_ERROR);
						}
						else{


							/*Check whether the timer is started already or not*/

							//In case the timer is not started
							if(gu8_StartedFlag == FALSE){
								//Start the timer
								aES_errorStatus = Timer_Start(TMU_TIMER_CH2,gstr_TMUcfg.Resolutions);
								//Mark that the timer is started successfully
								gu8_StartedFlag = TRUE;
							}
							//In case the timer is started
							else{
								//ERROR assignment
								aES_errorStatus = E_OK;
							}
						}

						break;

						default:
						//ERROR assignment
						aES_errorStatus = (E_INVALID_PARAMETER + TMU_ERROR);
						break;
					}

				}
			}
			}

			//if (aES_errorStatus != E_OK)
			else{
				//Do nothing
			}

		}
		//In case the task is a null pointer
		else{
			//ERROR assignment
			aES_errorStatus = (E_NULL_PTR + TMU_ERROR);
		}
	}
	//In case the module is uninitialized
	else{
		//ERROR assignment
		aES_errorStatus = (E_UNINITIALIZED_MODULE + TMU_ERROR);
	}

	//Return the error status
	return aES_errorStatus;
}

/**
 * Input:
 * 	taskID: the id of a specific task.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function removes the task with the given id.
 *
 */
ERROR_STATUS TMU_Stop_Timer(uint8_t taskID){
	/*Define Locals*/
	ERROR_STATUS aES_errorStatus;

	/*Check whether the module is initialized or not*/

	//In case the module has already been initialized
	if (gu8_InitFlag == TRUE)
	{
		//ERROR assignment
		aES_errorStatus = E_OK;

		/*Check if the TMU is started or not*/

		//In case the TMU is started
		if(gu8_StartedFlag == TRUE){
			//ERROR assignment
			aES_errorStatus = E_OK;

			/*Remove the task with the given task_id */
			aES_errorStatus = TMU_RemoveTask(taskID);

		}
		else{
			//ERROR assignment
			aES_errorStatus = (E_STOP_WITHOUT_START + TMU_ERROR);
		}
	}
	//In case the module is uninitialized
	else{
		//ERROR assignment
		aES_errorStatus = (E_UNINITIALIZED_MODULE + TMU_ERROR);
	}

	//Return the error status
	return aES_errorStatus;
}

/**
 * Input:
 * Output:
 * In/Out:
 * Return:
 * Description: This function act as the TMU Manager that manages the TMU task buffer and calls the TMU Consumers for each task
 *				 when the time is finished of this task
 *
 */
void TMU_Dispatcher(void){
	//Define locals
	uint16_t au16_TaskIterator;

	//In case there is an interrupt
	if(gu8_ISR_Flag == TRUE){
		//Reset the ISR FLAG
		gu8_ISR_Flag = FALSE;
		//Loop on all the objects in the buffer
		for(au16_TaskIterator =0; au16_TaskIterator < TMU_TASK_BUFFER_SIZE; au16_TaskIterator++){
			//In case an object exists in this index
			if (gstr_TasksBuffer[au16_TaskIterator] != NULL)
			{
				//Decrease the delay
				gstr_TasksBuffer[au16_TaskIterator]->Delay_Counter--;

				//In case the delay is finished
				if (gstr_TasksBuffer[au16_TaskIterator]->Delay_Counter == 0)
				{
					//Execute the callback function
					gstr_TasksBuffer[au16_TaskIterator]->TMU_Cbk_ptr();

					//In case the task is periodic reset the Counter
					if(gstr_TasksBuffer[au16_TaskIterator]->TMU_MODE == PERIODIC){
						gstr_TasksBuffer[au16_TaskIterator]->Delay_Counter = gstr_TasksBuffer[au16_TaskIterator]->Delay_TimeMs;
					}
					//In case the task is one shot function remove the task
					else{
						gstr_TasksBuffer[au16_TaskIterator]= NULL;
					}
				}
			}
		}
	}
}

/**
 * Input:
 * 	taskID: the id of a specific task.
 * Output:
 * In/Out:
 * Return:
 * Description: This function Removes the task with the given id from the buffer
 *
 */
static ERROR_STATUS TMU_RemoveTask(uint16_t TaskID){
	//Define Locals
	uint16_t au16_TaskIterator;
	ERROR_STATUS aES_errorStatus;

	for (au16_TaskIterator =0; au16_TaskIterator < TMU_TASK_BUFFER_SIZE; au16_TaskIterator++)
	{
	    if(gstr_TasksBuffer[au16_TaskIterator] != NULL){
            if (gstr_TasksBuffer[au16_TaskIterator]->Task_id == TaskID)
            {
                gstr_TasksBuffer[au16_TaskIterator] = NULL;
                aES_errorStatus = E_OK;
                break;
            }
            else{
                aES_errorStatus = (E_INVALID_PARAMETER + TMU_ERROR);
            }
	    }
	    else{
            aES_errorStatus = (E_INVALID_PARAMETER + TMU_ERROR);
	    }

	}
	return aES_errorStatus;
}

