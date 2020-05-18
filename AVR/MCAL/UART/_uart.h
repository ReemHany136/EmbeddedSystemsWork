#ifndef _UART_H_
#define _UART_H_
#include "std_types.h"


#define ENABLE 1
#define DISABLE 0

#define NINTH_BIT DISABLE

#define TXEN_MASK 1<<3
#define RXEN_MASK 1<<4

#define UDRE_FLAG_MASK	1<<5
#define TXC_FLAG_MASK	1<<6
#define RXC_FLAG_MASK	1<<7

/*BAUD RATE Configuration >> will be calculated*/
typedef enum En_Uart_BaudRate{
	UART_BR_9600=9600, UART_BR_115200=115200
}En_Uart_BaudRate;

/*Data bits Configuration >> UCSRC & USCRB */
typedef enum En_Uart_DataBits{
	_5BIT_DATA=0X00,_6BIT_DATA=0X02,_7BIT_DATA=0X04,_8BIT_DATA=0X06 ,_9BIT_DATA_A=0X06 ,_9BIT_DATA_B=0X04
}En_Uart_DataBits;

/*Parity Configuration >> UCSRC*/
typedef enum En_Uart_ParityBits{
	_ODD_PARITY=0X30,_EVEN_PARITY=0X20,_DIS_PARITY=0X00
}En_Uart_ParityBits;


/*Parity Configuration >> UCSRC*/
typedef enum En_Uart_StopBits{
	_1_STOP_BIT =0X00,_2_STOP_BITS =0X08
}En_Uart_StopBits;

/*Interrupt Configuration >> UCSRB*/
typedef enum En_Uart_Interrupt{
	_NO_POLLING =0X00,_TRANSMIT_UDRIE =0X20//,_TRANSMIT_RXCIE =0X80,_TRANSMIT_TXCIE =0X40
}En_Uart_Interrupt;


/*Interrupt Configuration >> UCSRB*/
typedef enum En_Uart_Speed{
	_NORMAL =0X00,_DOUBLE_SPEED=0x02
}En_Uart_Speed;

typedef struct UART_Config{
	En_Uart_BaudRate BaudRate;
	En_Uart_DataBits DataByte;
	En_Uart_ParityBits Parity;
	En_Uart_StopBits StopBits;
	En_Uart_Interrupt InterruptStatus;
	En_Uart_Speed Speed;
}UART_Config;

void _uartInit(UART_Config uartConfig);
//void _uartInit(uint32_t uartConfig);


void _uartTransmitChar(uint8_t Data);
void _uartTransmitStr(uint8_t* Data);

void _uartRecieveChar(uint8_t* Data);
void _uartTransmitInt(sint32_t Num);

void _uartAPP(void);
 
#endif /* _UART_H_ */