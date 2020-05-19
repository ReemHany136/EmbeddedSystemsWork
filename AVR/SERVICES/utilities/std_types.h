/*
 * std_types.h
 *
 *  Created on: Oct 21, 2019
 *      Author: Sprints
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_


/************************************************************************/
/*                   typedefs for standard types                        */
/************************************************************************/

#define NULL	((void *)0)

typedef unsigned char uint8__t;
typedef unsigned short int uint16__t;
typedef unsigned long int uint32__t;
typedef unsigned long long uint64__t;
typedef signed char sint8_t;
typedef signed int sint16_t;
typedef signed long int sint32_t;
typedef signed long long sint64_t;
typedef volatile uint8__t* const reg_type8_t;
typedef volatile uint16__t* const reg_type16_t;
typedef sint16_t ERROR_STATUS;

/************************************************************************/
/*				 		defining Microcontroller clock                      */
/************************************************************************/
#define F_CPU 16000000.0

/************************************************************************/
/*				 		defining boolean values                         */
/************************************************************************/
#define FALSE 	0	// defines boolean false
#define TRUE 	1	// defines boolean true

#define ZERO    0
#define ONE     1
#define TWO     2

/************************************************************************/
/*						 LOW/HIGH defines                               */
/************************************************************************/
#define LOW  0		// defines LOW value for a bit
#define HIGH 0xFF	// defines HIGH value for a bit
#define OUTPUT			0xFF
#define INPUT			0x00

#endif /* STD_TYPES_H_ */