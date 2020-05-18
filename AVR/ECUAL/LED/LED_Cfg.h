/*
 * LED_Cfg.h
 *
 * Created: 4/12/2020 6:45:21 PM
 *  Author: Reem
 */ 


#ifndef LED_CFG_H_
#define LED_CFG_H_

#include "../../MCAL/DIO/DIO.h"

/*
 * This macro define the maximum used leds
 */
#define LED_MAX_NUM	(4)

/*
 * LED_x_GPIO and LED_x_BIT
 * where x is the led identifier
 */
#define LED_0_GPIO	(GPIOB)
#define LED_0_BIT	(BIT4)

#define LED_1_GPIO	(GPIOB)
#define LED_1_BIT	(BIT5)

#define LED_2_GPIO	(GPIOB)
#define LED_2_BIT	(BIT6)

#define LED_3_GPIO	(GPIOB)
#define LED_3_BIT	(BIT7)


#endif /* LED_CFG_H_ */