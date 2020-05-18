
#include "utilities.h"
#include "registers.h"
#include "interrupts.h"
#include "gpio.h"
#include "_uart.h"
#include "led.h"

static volatile uint16_t dataCounter=0;
static volatile uint8_t dataByteTransmit;
static UART_Config uartConfig_Internal;
static uint8_t BaudRateDivisor=16;


static void _uart_BRFactor_Calc(uint32_t BaudRate){
	UBRRL = (uint8_t)((16000000.0/(BaudRateDivisor*BaudRate))-1);
}
void _uartInit(UART_Config uartConfig){
	uartConfig_Internal = uartConfig;
	/*Adjust the speed*/
	
	UCSRA |=uartConfig.Speed;
	if(uartConfig.Speed == _DOUBLE_SPEED){
		BaudRateDivisor =8;
	}
	else{
		BaudRateDivisor =16;
	}
	
	/*EN TX & RX*/
	
	UCSRB |= TXEN_MASK ;
	UCSRB |= RXEN_MASK;

	
	/*Adjust No. of data bits , Stop bits and Parity*/

	
	UCSRC |= uartConfig.DataByte|uartConfig.StopBits|uartConfig.Parity;
	
	 if (NINTH_BIT == ENABLE)
	 {
		 UCSRB |= _9BIT_DATA_B;
	 }
	 
	 if (uartConfig.InterruptStatus != _NO_POLLING)
	 {
		 EN_GLOBAL_INT;
	 }
	
	/*Adjust Baud Rate*/
	_uart_BRFactor_Calc(uartConfig.BaudRate);
	//UBRRL =51;
	UBRRH =0;
	gpioPinDirection(GPIOD,BIT0,INPUT);
	gpioPinDirection(GPIOD,BIT1,OUTPUT);
	//EN_GLOBAL_INT;
}
void _uartTransmitChar(uint8_t Data){
	if(uartConfig_Internal.InterruptStatus == _TRANSMIT_UDRIE){
		UCSRB |= _TRANSMIT_UDRIE;
		
		dataByteTransmit = Data;
	}
	else{
		while(!(UCSRA & UDRE_FLAG_MASK));
		UDR = Data;
		if(NINTH_BIT == ENABLE){
			
		}
	}
	

}
void _uartTransmitStr(uint8_t* Data){
	dataCounter =0;
	if(uartConfig_Internal.InterruptStatus == _NO_POLLING){
		while(Data[dataCounter]!='\0'){
			
			_uartTransmitChar(Data[dataCounter]);
			dataCounter++;
		}
	}
	else{
		while(Data[dataCounter]!='\0'){
		
		_uartTransmitChar(Data[dataCounter]);
		clr_bit(UCSRB,5);
	}
	}
	
}
void _uartTransmitInt(sint32_t Num){
	sint32_t number[32];
	sint8_t counter =0,size=0;
	if(Num == 0){
		_uartTransmitChar('0');
	}
	else{
		if(Num<0){
			_uartTransmitChar('-');
			Num=Num*-1;
		}

		while(Num>0){

			number[size] = Num%10;
			size++;
			Num = Num/10;
		}
		size--;
		while(size>=0){
			_uartTransmitChar(number[size]+48);
			counter++;
			size--;
		}
	}
 
}

void _uartRecieveChar(uint8_t* Data){
	while((UCSRA & RXC_FLAG_MASK)==0);//lesa el buffer mana2alsh el data lel transmit shift reg
	(*Data) = UDR;
}

void _uartAPP(void){
	
	UART_Config uart_config;
	uint8_t recievedChar;
	
	uart_config.BaudRate = 9600;
	uart_config.DataByte =_8BIT_DATA;
	uart_config.InterruptStatus = _NO_POLLING;
	uart_config.Parity = _DIS_PARITY;
	uart_config.StopBits = _1_STOP_BIT;
	uart_config.Speed= _DOUBLE_SPEED;
	
	Led_Init(LED_0);
	Led_Init(LED_1);
	Led_Init(LED_2);
	Led_Init(LED_3);
	
	_uartInit(uart_config);
	_uartTransmitStr("Please Enter the number of the led (0~3) to toggle the led with the same Number: ");
	while(1){
		_uartRecieveChar(&recievedChar);
		if(recievedChar == '0'){
			Led_Toggle(LED_0);
		}
		else if(recievedChar == '1'){
			Led_Toggle(LED_1);
		}
		else if(recievedChar == '2'){
			Led_Toggle(LED_2);
		}
		else if(recievedChar == '3'){
			Led_Toggle(LED_3);
		}
		
	}
}

_ISR__(USART_UDRE_vect){
	
	UDR = dataByteTransmit;
	clr_bit(UCSRB,5);
	dataCounter++;
	
}

