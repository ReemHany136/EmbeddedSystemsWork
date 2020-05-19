/*author : a7md Salama
*/

/*includes*/
//#include "_stdint.h"
#include "Clock.h"
#include "Clocklcfg.h"
#include "PerRegs.h"

/*Defines*/
#define HSE_CLK_HZ                   (8)
#define HSI_CLK_HZ                   (16)
#define MAX_CLOCK_MHZ             (168)
#define MIN_CLOCK_MHZ             (25)
#define RCC_HSEREADY              (1<<17)
#define RCC_HSEON                 (1<<16)
#define RCC_CSS_ON                (0x80000)
#define RCC_HSION                 (0x01)
#define RCC_HSIREADY              (0x02)

#define PLL_RESET_VALUE           0x24003010
/*P = 4 and M =2*/
#define PLL_P_FACTOR              (0x00020000)
#define PLL_M_FACTOR              (0x00000004)
#define PLL_MAX_N_FACTOR          (511)
#define PLL_MAX_M_FACTOR          (63)
#define PLL_MAX_P_FACTOR          (8)

#define PLL_SOURCE                (0x400000)

#define APB1_PRES                 (0b101 << 10)
#define APB2_PRES                 (0b100 << 13)

#define HSI_SWITCH                (0)
#define HSE_SWITCH                (1)
#define PLL_SWITCH                (2)
#define CLK_SWITCH                (PLL_SWITCH)
#define CLK_SWITCH_MASK           (0b11)
#define PLL_ON                    (0x1000000)
#define PLL_READY                 (0x2000000)
 
#define RCC_PLL_SYSCLOCK          (0x02)

/*Buses Prescal*/
#define RCC_APB2_90MHZ_FACTOR     (0x04)
#define RCC_APB1_45MHZ_FACTOR     (0x05)

#define PLL_N_WRONG_CFG_0         (0X00)
#define PLL_N_WRONG_CFG_1         (0X01)
#define PLL_N_WRONG_CFG_433       (433)
#define PLL_N_WRONG_CFG_511       (511)

#define PLL_M_WRONG_CFG_0         (0X00)
#define PLL_M_WRONG_CFG_1         (0X01)

/*APIS*/

/*ClockInit
*Description : function that inits clock with desired frequency
*Input : None
*Output : ERROR if found
*/
ERROR_STATUS Clock_init(void)
{
    /*Create ERROR FLAG*/
    ERROR_STATUS ERR_STATE = E_OK;

    /*Check for Clock Value*/
    if(str_ck_init.Clock_Frequency_MHZ > ((uint32_t) MAX_CLOCK_MHZ) && str_ck_init.Clock_Frequency_MHZ < ((uint32_t) MIN_CLOCK_MHZ))
    {
        /*Record error*/
        ERR_STATE = ERROR_SYSTICK_BASE + ERROR_WRONG_CLOCK_VALUE;
    }
    else{

    	uint16_t n,m,p,p_mask;
    	uint32_t u32PLLCalcultedOuputClk,PLL_P_Factor,PLL_M_Factor,PLL_N_Factor;

    	switch(str_ck_init.Clock_Source)
    	{
    			case HSE_CLK:
    	    		break;
    	    	case PLL_HSE_CLK:
    	    		/*1- EN The requested CLK Mode*/
    	    		RCC->CR |= RCC_HSION;
    	    		/*2- wait The requested CLK Mode to be ready*/
    	    		while(!(RCC->CR & RCC_HSIREADY));

    	    		/*3- Switch to the HSI CLK*/
    	    		RCC->CFGR &= ~(CLK_SWITCH_MASK);
    				RCC->CFGR |= HSI_SWITCH;

    	    		/*4- Configure the PLL*/
    	    		// 1- Disable PLL
    	    		RCC->CR &= ~(PLL_ON);
    	    		// 2- Set the PLL input CLK to HSE
    	    		RCC->PLLCFGR |= PLL_SOURCE;
    	    		// 3- Set the PLL Factors
    	    		for(p = 2,p_mask=0; p <= 8 ; p+=2,p_mask++)
    				{
    	    			for(m = 0; m < PLL_MAX_M_FACTOR ; m++)
    					{
    	    				if(m == PLL_M_WRONG_CFG_0 || m == PLL_M_WRONG_CFG_1)
    	    				{
    	    					continue;
    	    				}
    	    				else
    	    				{
    							for(n = 0; n <= PLL_MAX_N_FACTOR ; n++)
    							{
    								if(n == PLL_N_WRONG_CFG_0 || n == PLL_N_WRONG_CFG_1 ||
    								n == PLL_N_WRONG_CFG_433 || n == PLL_N_WRONG_CFG_511)
    								{
    									continue;
    								}
    								else
    								{
    									u32PLLCalcultedOuputClk = (uint32_t)(((HSE_CLK_HZ)*(n/m))/p);
    									if(u32PLLCalcultedOuputClk == str_ck_init.Clock_Frequency_MHZ)
    									{
    										PLL_N_Factor = n<<6;
    										PLL_M_Factor = m;
    										PLL_P_Factor = p_mask<<16;
    										p=1000;
    										m=1000;
    										n=1000;
    										break;
    									}
    								}
    							}
    	    				}
    					}
    				}
    	    		//Write the selected factors in the pllCfg register
    	    		RCC->PLLCFGR |= PLL_N_Factor | PLL_M_Factor | PLL_P_Factor;

    	    		// 4- Enable PLL
    	    		RCC->CR |= PLL_ON;
    	    		// 5- Wait for the PLL to be ready
    	    		while(!(RCC->CR & PLL_READY));

    	    		/*5- Configure the Buses*/
    	    		RCC->CFGR |= APB1_PRES | APB2_PRES;

    	    		/*6- EN The requested CLK Mode*/
    				RCC->CR |= RCC_HSEON;
    				/*7- wait The requested CLK Mode to be ready*/
    				while(!(RCC->CR & RCC_HSEREADY));
    	    		/*8- Switch to the configured CLK*/
    				RCC->CFGR &= ~(CLK_SWITCH_MASK);
    	    		RCC->CFGR |= CLK_SWITCH;

    	    		break;

    	}

    }

    return ERR_STATE;
}

#ifdef COMMENT

/*Create N Factor Var*/
        uint16_t PLL_N_Factor;

        /*Clear CR REG first*/
        RCC->CR = ((0x00000083));;

        /*Enable HSE and HSI*/
        RCC->CR |= (uint32_t) RCC_HSION;
        while((RCC->CR & ((uint32_t)RCC_HSIREADY)) == ZERO);

        /*wait for RCC Ready Flag*/
        RCC->CR |= (uint32_t) RCC_HSEON;
        while((RCC->CR & ((uint32_t)RCC_HSEREADY)) == ZERO);

        /*Enable CSS*/
        RCC->CR |= RCC_CSS_ON;

        /*Set PLL Parameters*/
        /*Calculate N Factor*/
         PLL_N_Factor = (8 * str_ck_init.Clock_Frequency_MHZ)/ 8;

         if(PLL_N_Factor > PLL_MAX_N_FACTOR)
          {
               ERR_STATE = ERROR_SYSTICK_BASE +  ERROR_WRONG_N_FACTOR;
          }
           else
          {
                    /* assign  N_Fact value */
           RCC->PLLCFGR = 0x7405400;//((PLL_N_Factor << 6) | PLL_P_FACTOR | 2 | PLL_HSE_SOURCE);
          }

        /*P=4 Factor and M = 2*/
        //RCC->PLLCFGR = PLL_RESET_VALUE;
        //RCC->PLLCFGR |= (((uint32_t)PLL_P_FACTOR) | ((uint32_t)PLL_M_FACTOR));

        /*Choose HSE AS PLL Source*/
        //RCC->PLLCFGR |= (uint32_t)PLL_HSE_SOURCE;
        
        /*Enable PLL and wait till it's enabled*/
        RCC->CR |= (uint32_t) PLL_ON;
        while((RCC->CR & PLL_READY) == ZERO);

        /*Choose PLL As Sys Clock*/
        RCC->CFGR |= (uint32_t) RCC_PLL_SYSCLOCK;

        /*SET APB1,APB2 Speeds*/
        RCC->CFGR |= (uint32_t)((RCC_APB1_45MHZ_FACTOR << 10) | (RCC_APB2_90MHZ_FACTOR << 13));
#endif



