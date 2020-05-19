#include "../includes/SysCtrl_lCfg.h"
#include "../includes/MCU_hw.h"
strSysCtlCfg_s_t periph_Clk[NUM_OF_PRIPHERALS] ={
RCGC_GPIOE         ,         &SYSCTL_RCGCGPIO_R,
RCGC_GPIOF         ,         &SYSCTL_RCGCGPIO_R,
RCGC_ADC_MOD_0     ,         &SYSCTL_RCGCADC_R,
};

