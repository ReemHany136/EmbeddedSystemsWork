/*
 * SPI__.h
 *
 * Created: 2/10/2020 6:54:43 PM
 *  Author: MENA
 */ 


#ifndef SPI___H_
#define SPI___H_


#include "std_types.h"
#include "registers.h"
#include "common_macros.h"
#include "interrupts.h"

#define ZERO                  0 

#define ENABLE                1
#define DISABLE               0


#define SCK_PORT              GPIOB
#define MISO_PORT             GPIOB
#define MOSI_PORT             GPIOB
#define SS_PORT               GPIOB

#define SCK_PIN               BIT7
#define MISO_PIN              BIT6
#define MOSI_PIN              BIT5
#define SS_PIN                BIT4

#define Enable_SPI            0x40

#define Master_SPI            0x10
#define Slave_SPI             0x00

#define SPI_INTERRUPT_MODE    0X80
#define SPI_POLLING_MODE      0x00

#define SPI_TRANSMIT          0
#define SPI_RECIEVE           1

typedef uint8_t SPI_Interrupt_t;

typedef enum SPI_clk{
	Fosc4 = 0x00 ,
	Fosc16 = 0x01 ,
	Fosc64 = 0x02 ,
	Fosc128 = 0x03 
	}SPI_clk;

typedef enum SPI_mode{
	mode1 = 0x00 ,
	mode2 = 0x04 ,
	mode3 = 0x08 ,
	mode4 = 0x0C 
}SPI_mode;

typedef enum SPI_DataOrder{
	MSB = 0x00 ,
	LSB = 0x20
	
	}SPI_DataOrder;




extern void _SPIInitMaster(SPI_clk clock ,SPI_mode phasePolarityMode,SPI_DataOrder dataorder, SPI_Interrupt_t InterruptMode);
extern void _SPIInitSlave(SPI_clk clock ,SPI_mode phasePolarityMode,SPI_DataOrder dataorder ,SPI_Interrupt_t InterruptMode ,void (*SPI_Cbk)(uint8_t));


extern void _SPITrancevier(uint8_t data,uint8_t *ptrData);



#endif /* SPI___H_ */