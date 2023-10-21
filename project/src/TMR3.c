#include "TMR3.h"
#include "at32f421_wk_config.h"

void TMR3_Stop()
{
	tmr_flag_clear(TMR3, TMR_OVF_FLAG);
	tmr_counter_enable(TMR3, FALSE);
}


void TMR3_Start(uint32_t time)
{
	tmr_counter_value_set(TMR3, time);
	tmr_counter_enable(TMR3, TRUE);
}