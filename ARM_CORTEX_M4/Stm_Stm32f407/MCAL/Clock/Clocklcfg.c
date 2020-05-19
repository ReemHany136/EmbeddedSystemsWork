
#include "Clocklcfg.h"

#define MAX_FREQUENCY 168

/*define Clock inits here*/
volatile str_ckInit_t str_ck_init = {PLL_HSE_CLK , MAX_FREQUENCY};
