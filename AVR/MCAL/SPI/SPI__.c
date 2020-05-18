/*
 * SPI__.c
 *
 * Created: 2/10/2020 6:54:24 PM
 *  Author: MENA
 */ 

#include "SPI__.h"
#include "DIO.h"
#include "softwareDelay.h"

/*Globals*/
 volatile uint8_t gu8_InterruptMode, gu8_TranscieveFlag;
 volatile uint8_t gu8_Data,ISR_RECIEVE_FLAG = FALSE;
 volatile void (*ptrSPICbk)(uint8_t)=NULL;
/**/

void _SPIInitMaster(SPI_clk clock ,SPI_mode phasePolarityMode, SPI_DataOrder dataorder,SPI_Interrupt_t InterruptMode ){
	DIO_Cfg_s astr_DioCfg;
	//// clear status register
	SPSR = ZERO ;
		
	/// clear control mode
		
	SPCR = ZERO ;
		
	// master or slave bit 4 
	SPCR |= Master_SPI ; 
		
	//set clock
		 
	SPCR|=clock ;
		
	/// data order update
		
	SPCR|=dataorder ;
		
	// set phase bit2 and polarity bit3
		
	SPCR |= phasePolarityMode;
	
	//set the interrupt Mode whether it's interrupt or polling
	SPCR |= InterruptMode;
	gu8_InterruptMode = InterruptMode;
	if(InterruptMode == SPI_INTERRUPT_MODE){
		EN_GLOBAL_INT;
		
	}
	////// GPIO set output and input 
	 // ss bit4
	 astr_DioCfg.GPIO =SS_PORT;
	 astr_DioCfg.pins =SS_PIN|MOSI_PIN|SCK_PIN;
	 astr_DioCfg.dir =OUTPUT;
	 DIO_init(&astr_DioCfg);
	
	astr_DioCfg.GPIO =SS_PORT;
	astr_DioCfg.pins =MISO_PIN;
	astr_DioCfg.dir =INPUT;
	DIO_init(&astr_DioCfg);
	/// enable SPI bit 6
	SPCR |= Enable_SPI ;
}

void _SPIInitSlave(SPI_clk clock ,SPI_mode phasePolarityMode,SPI_DataOrder dataorder ,SPI_Interrupt_t InterruptMode ,void (*SPI_Cbk)(uint8_t)){
	

	DIO_Cfg_s astr_DioCfg;

	//// clear status register
	SPSR = ZERO ;
	
	/// clear control mode
	
	SPCR = ZERO ;
	
	
	// master or slave bit 4
	SPCR |= Slave_SPI ;
	
	/// data order update
	
	SPCR|=dataorder ;
	
	//set clock
	
	SPCR|=clock ;
	
	// set phase and polarity
	SPCR |=phasePolarityMode ;
	
	//set the interrupt Mode whether it's interrupt or polling
	SPCR |= InterruptMode;
	gu8_InterruptMode = InterruptMode;
	if(InterruptMode == SPI_INTERRUPT_MODE){
		EN_GLOBAL_INT;
		ptrSPICbk = SPI_Cbk;
		
	}
	
	////// GPIO set output and input
	 // ss bit4
	 astr_DioCfg.GPIO =SS_PORT;
	 astr_DioCfg.pins =SS_PIN|MOSI_PIN|SCK_PIN;
	 astr_DioCfg.dir =INPUT;
	 DIO_init(&astr_DioCfg);
	 
	 astr_DioCfg.GPIO =SS_PORT;
	 astr_DioCfg.pins =MISO_PIN;
	 astr_DioCfg.dir =OUTPUT;
	 DIO_init(&astr_DioCfg);

	
	/// enable SPI bit 6
	SPCR |= Enable_SPI ;
	
}

void _SPITrancevier(uint8_t data,uint8_t *ptrData){
	
	if(SPCR&Master_SPI){
		
	
		
		DIO_Write(GPIOB,BIT4,LOW);
		
//		timer0DelayMs(1);

	}
	if (gu8_InterruptMode == SPI_INTERRUPT_MODE){
		SPDR = (data);
	//	gu8_Data =data;
		while (!ISR_RECIEVE_FLAG);
		(*ptrData) = 255;
		
		
		ISR_RECIEVE_FLAG = FALSE;
		
	}
	else if (gu8_InterruptMode == SPI_POLLING_MODE)
	{
		SPDR = (data);
			
		while (!(SPSR&(1<<7)));
		(*ptrData) = SPDR;	
	}
	 
	
	
	
	
}



_ISR__(SPI_STC_vect){
	
	gu8_Data = SPDR;
	ISR_RECIEVE_FLAG = TRUE;
	TCNT2 = SPDR ;
	if(ptrSPICbk!=NULL){
		
		ptrSPICbk(SPDR);
		
	}
}