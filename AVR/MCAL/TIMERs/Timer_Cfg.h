/*
 * Timer_Cfg.h
 *
 * Created: 2/16/2020 10:52:31 PM
 *  Author: Reem
 */ 


#ifndef TIMER_CFG_H_
#define TIMER_CFG_H_

#define     ENABLE                        0
#define     DISABLE                       1

/************************************************/
/*        Timer Flag Register (TIFR) pins              */
/************************************************/

#define TOV_T0					1<<0
#define OCF0_T0					1<<1
#define TOV_T1					1<<2
#define OCFA_T1					1<<3
#define OCFB_T1					1<<4
#define ICF1					1<<5
#define TOV_T2					1<<6
#define OCF2_T2					1<<7


/************************************************/
/*        Timer0 CTRL definition                */
/************************************************/
#define     TIMER0_OVF_WMODE              ENABLE
#define     TIMER0_CTC_WMODE              DISABLE


// Define Timer 0 Interrupt masks 
#define     T0_POLLING_MASK               0
#define     T0_INTERRUPT_NORMAL_MASK      0x01
#define     T0_INTERRUPT_CTC_MASK         0x02

// Define Timer 0 OVF Mode 
#if TIMER0_OVF_WMODE == ENABLE
	#define T0_WGMODE_MASK                0
	#define T0_OC0_MASK                   0
	#define T0_INTERRUPT_MASK             T0_INTERRUPT_NORMAL_MASK
	#define T0_INT_FLAG                   TOV_T0

// Define Timer 0 CTC Mode 
#elif TIMER0_CTC_WMODE == ENABLE
	#define T0_WGMODE_MASK                0x08
	//#define T0_OC0_MASK                   0x20
	#define T0_OC0_MASK                   0
	#define T0_INTERRUPT_MASK             T0_INTERRUPT_CTC_MASK
	#define T0_INT_FLAG                   OCF0_T0
	
#endif

#define RESET_INT_TIMER0_FLAG             TIFR |= 1<<T0_INT_FLAG;

// Define Timer 0 Prescale masks 
#define     T0_NO_CLOCK                   0xF8
#define     T0_PRESCALER_NO               0x01
#define     T0_PRESCALER_8                0x02
#define     T0_PRESCALER_64               0x03
#define     T0_PRESCALER_256              0x04
#define     T0_PRESCALER_1024             0x05





/************************************************/
/*        Timer1 CTRL definition                */
/************************************************/
#define     TIMER1_OVF_WMODE              ENABLE
#define     TIMER1_CTC_WMODE              DISABLE

// Define Timer 1 Interrupt masks
#define     T1_POLLING_MASK               0
#define     T1_INTERRUPT_NORMAL_MASK      0x04
#define     T1_INTERRUPT_CTC1A_MASK       0x10
#define     T1_INTERRUPT_CTC1B_MASK       0x08

// Define Timer 1 OVF Mode 
#if TIMER1_OVF_WMODE == ENABLE
	#define T1_WGMODE_MASK                0
	#define T1_OC1_MASK                   0
	#define T1_INTERRUPT_MASK             T1_INTERRUPT_NORMAL_MASK
	#define T1_INT_FLAG                   TOV_T1
	
// Define Timer 1 CTCA Mode 
#elif TIMER1_CTC_WMODE == ENABLE
	#define T1_WGMODE_MASK                0x0008
	#define T1_OC1_MASK                   0
	//#define T1_OC1_MASK                   0x8000
	#define T1_INTERRUPT_MASK             T1_INTERRUPT_CTC1A_MASK
	#define T1_INT_FLAG                   OCFA_T1
	
#endif

#define RESET_INT_TIMER1_FLAG             TIFR |= 1<<T1_INT_FLAG;

// Define Timer 1 Prescale masks
#define     T1_NO_CLOCK                   0xF8
#define     T1_PRESCALER_NO               0x0001
#define     T1_PRESCALER_8                0x0002
#define     T1_PRESCALER_64               0x0003
#define     T1_PRESCALER_256              0x0004
#define     T1_PRESCALER_1024             0x0005




/************************************************/
/*        Timer2 CTRL definition                */
/************************************************/
#define     TIMER2_OVF_WMODE              DISABLE
#define     TIMER2_CTC_WMODE              ENABLE 

// Define Timer 2 Interrupt masks
#define     T2_POLLING_MASK               0
#define     T2_INTERRUPT_NORMAL_MASK      0x40
#define     T2_INTERRUPT_CTC_MASK         0x80

// Define Timer 2 OVF Mode 
#if TIMER2_OVF_WMODE == ENABLE
	#define T2_WGMODE_MASK                0
	#define T2_OC2_MASK                   0
    #define T2_INTERRUPT_MASK             T2_INTERRUPT_NORMAL_MASK
	#define T2_INT_FLAG                   TOV_T2
	
// Define Timer 2 CTC Mode 
#elif TIMER2_CTC_WMODE == ENABLE
	#define T2_WGMODE_MASK                0x08
	#define T2_OC2_MASK                   0
	/*#define T2_OC2_MASK                   0x20*/
	#define T2_INTERRUPT_MASK             T2_INTERRUPT_CTC_MASK
	#define T2_INT_FLAG                   OCF2_T2
	
#endif

#define RESET_INT_TIMER2_FLAG             TIFR |= 1<<T2_INT_FLAG;


// Define Timer 2 Prescale masks
#define     T2_NO_CLOCK                   0xF8
#define     T2_PRESCALER_NO               0x01
#define     T2_PRESCALER_8                0x02
#define     T2_PRESCALER_32               0x03
#define     T2_PRESCALER_64               0x04
#define     T2_PRESCALER_128              0x05
#define     T2_PRESCALER_256              0x06
#define     T2_PRESCALER_1024             0x07


#endif /* TIMER_CFG_H_ */