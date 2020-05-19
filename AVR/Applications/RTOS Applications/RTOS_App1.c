#include "Services/FreeRTOS/FreeRTOS.h"
#include "Services/FreeRTOS/task.h"
#include "Services/FreeRTOS/semphr.h"
#include "HAL/LED/LED.h"
#include "HAL/PushButton/pushButton.h"
#include "HAL/LCD/LCD.h"

/********************** User Story 1 *****************/

/*____________________________________________*/
/*______________Static Globals _______________*/
/*____________________________________________*/
TaskHandle_t xHandle ;
TaskHandle_t xHandle1;
TaskHandle_t xHandle2;
TaskHandle_t xHandle3;
SemaphoreHandle_t xSemaphore;
uint8__t gau8_SharedStr[100];
/*____________________________________________*/
/*______________Task definition_______________*/
/*____________________________________________*/

/*
 * Task: SystemInit_Task
 * Task description: This Task used for initializing the used modules and globals
 */
void SystemInit_Task(void)
{
	//initializing globals
	
	
	while(1)
	{
		//initializing modules
		//Led_Init(LED_0);
		pushButtonInit(BTN_0);
 		LCD_init(DATA_BITS_4);
// 		LCD_displayStringRowColumn(0,0,"HHHHHHHHH");
		//Suspend this task to avoid repeating it again
		vTaskSuspend( xHandle2 );
	}
	
	
	
}


/*
 * Task: LCD_Display_TaskA
 * Task description: This Task used for displaying on the lcd the given string  
 *                  pushing the btn
 */
void LCD_Display_TaskA(void)
{
	TickType_t xLastWakeTime;
	const TickType_t xdelayTicks = 200;
	while (1)
	{
		if(!(xSemaphoreTake(xSemaphore,(TickType_t) 10)))
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,gau8_SharedStr);
			LCD_displayStringRowColumn(1,0,"I have been overwritten!");
			TCNT2 =0X93;
		}
		else
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"HHHH");
			TCNT2 =0X39;
		}
		vTaskDelayUntil( &xLastWakeTime, xdelayTicks );
		LCD_clearScreen();
	}
	
}

/*
 * Task: BTN_Task
 * Task description: This Task used for detecting whether the button is pressed
 *                   and released to take action with the led
 */
void BTN_TaskB(void)
{
	TickType_t xLastWakeTime;
	const TickType_t xdelayTicks = 50;
	static uint8__t u8ledperiodicityCounter;
	uint16_t u16pressingPeridicity = 50;
	static uint8__t u8lessFlag = TRUE;
	while(1)
	{
		// In case the btn is pressed
		if(pushButtonGetStatus(BTN_0) == Pressed)
		{
			if(xSemaphoreTake(xSemaphore,(TickType_t)10));
		}
		else
		{
			xSemaphoreGive(xSemaphore);
		}
		vTaskDelayUntil( &xLastWakeTime, xdelayTicks );
	}
}

/*
 * Task: BTN_Task
 * Task description: This Task used for detecting whether the button is pressed
 *                   and released to take action with the led
 */
void SendStr_TaskC(void)
{
	TickType_t xLastWakeTime;
	const TickType_t xdelayTicks = 50;
	static uint8__t u8ledperiodicityCounter;
	uint16_t u16pressingPeridicity = 400;
	static uint8__t u8lessFlag = TRUE;
	uint8__t u8_i, au8str[]="Hello LCD";
	while(1)
	{
		for (u8_i=0;u8_i<10;u8_i++)
		{
			gau8_SharedStr[u8_i]=au8str[u8_i];
		}
		
		vTaskDelayUntil( &xLastWakeTime, xdelayTicks );
	}
}





int main(void)
{
	
    BaseType_t ret_,ret_1,ret_2,ret_3,ret_4;
	xHandle  = NULL;
	xHandle1 = NULL;
	xHandle2 = NULL;
	xHandle3 = NULL;
	/*Creating Semaphore*/
	xSemaphore = xSemaphoreCreateBinary();
	/*Creating tasks*/
    ret_1= xTaskCreate(LCD_Display_TaskA   , "LCD_Display" , 100 , NULL , 1 , &xHandle3);
    ret_2= xTaskCreate(SendStr_TaskC   , "SendStr" , 100 , NULL , 2 , &xHandle);
    ret_3= xTaskCreate (BTN_TaskB       , "BTN"     , 100 , NULL , 3 , &xHandle1);
	ret_4= xTaskCreate (SystemInit_Task, "sysInit" , 100  , NULL , 4 , &xHandle2);
	/* Start Scheduler */
	//vTaskStartScheduler();
	LCD_init(DATA_BITS_4);
	LCD_displayString("HHHHHHHHH");

	while (1)
	{
	}
	return 0;
}
