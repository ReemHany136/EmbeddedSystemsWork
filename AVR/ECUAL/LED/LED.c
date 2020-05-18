/*
 * LED.c
 *
 * Created: 4/12/2020 6:45:40 PM
 *  Author: Reem
 */ 
#include "../../MCAL/DIO/DIO.h"
#include "LED_Cfg.h"
#include "LED.h"
/**
 * Description: Initialize the LED_x (where x 0, 1, 2, 3) as output
 * @param led_id: The led to be initialized and it takes
 * 				  one of the enum (En_LedNumber_t) parameters
 *
 */
void Led_Init(En_LedNumber_t en_led_id){
	st_DIO_Cfg_t astrDioCfg;
	switch(en_led_id){
		case LED_0:
		   astrDioCfg.GPIO = LED_0_GPIO;
		   astrDioCfg.pins = LED_0_BIT;
		 
		break;
		case LED_1:
			 astrDioCfg.GPIO = LED_1_GPIO;
			 astrDioCfg.pins = LED_1_BIT;
		break;
		case LED_2:
			 astrDioCfg.GPIO = LED_2_GPIO;
			 astrDioCfg.pins = LED_2_BIT;
		break;
		case LED_3:
			 astrDioCfg.GPIO = LED_3_GPIO;
			 astrDioCfg.pins = LED_3_BIT;
		break;
	}
	astrDioCfg.dir = OUTPUT;
	DIO_init(&astrDioCfg);
}
/**
 * Description: Change the LED_x state (where x 0, 1, 2, 3) to be on
 * @param led_id: The led to be initialized and it takes
 * 				  one of the enum (En_LedNumber_t) parameters
 *
 */
void Led_On(En_LedNumber_t en_led_id){
	switch(en_led_id){
		case LED_0:
		DIO_Write(LED_0_GPIO,LED_0_BIT,HIGH);
		break;
		case LED_1:
		DIO_Write(LED_1_GPIO,LED_1_BIT,HIGH);
		break;
		case LED_2:
		DIO_Write(LED_2_GPIO,LED_2_BIT,HIGH);
		break;
		case LED_3:
		DIO_Write(LED_3_GPIO,LED_3_BIT,HIGH);
		break;
	}
}
/**
 * Description: Change the LED_x state (where x 0, 1, 2, 3) to be off
 * @param led_id: The led to be initialized and it takes
 * 				  one of the enum (En_LedNumber_t) parameters
 *
 */
void Led_Off(En_LedNumber_t en_led_id){
	switch(en_led_id){
		case LED_0:
		DIO_Write(LED_0_GPIO,LED_0_BIT,LOW);
		break;
		case LED_1:
		DIO_Write(LED_1_GPIO,LED_1_BIT,LOW);
		break;
		case LED_2:
		DIO_Write(LED_2_GPIO,LED_2_BIT,LOW);
		break;
		case LED_3:
		DIO_Write(LED_3_GPIO,LED_3_BIT,LOW);
		break;
	}
}
/**
 * Description: Toggle the LED_x state (where x 0, 1, 2, 3)
 * @param led_id: The led to be initialized and it takes
 * 				  one of the enum (En_LedNumber_t) parameters
 *
 */
void Led_Toggle(En_LedNumber_t en_led_id){
	switch(en_led_id){
		case LED_0:
		DIO_Toggle(LED_0_GPIO,LED_0_BIT);
		break;
		case LED_1:
		DIO_Toggle(LED_1_GPIO,LED_1_BIT);
		break;
		case LED_2:
		DIO_Toggle(LED_2_GPIO,LED_2_BIT);
		break;
		case LED_3:
		DIO_Toggle(LED_3_GPIO,LED_3_BIT);
		break;
	}
}