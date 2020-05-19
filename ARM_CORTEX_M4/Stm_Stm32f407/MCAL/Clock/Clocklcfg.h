/*Author : A7md Salama
*/
/*create Structure of clock configs*/
#include "std_types.h"

/*Defines*/
#define HSI_CLK           0
#define HSE_CLK           1
#define PLL_HSI_CLK       2
#define PLL_HSE_CLK       3


typedef struct{
    uint8_t Clock_Source;
    uint8_t Clock_Frequency_MHZ;
}str_ckInit_t;


volatile extern str_ckInit_t str_ck_init;
