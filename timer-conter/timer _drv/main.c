#include <nrf.h>
#include <stdbool.h>
#include <bsp.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>

#include <nrf_timer.h>
#include <nrf_drv_timer.h>
#include <app_util_platform.h>

nrf_drv_timer_t timer=NRF_DRV_TIMER_INSTANCE(0);
nrf_drv_timer_config_t timerConfig=NRF_DRV_TIMER_DEFAULT_CONFIG(0);

void timer_event_handler(nrf_timer_event_t event_type,void * p_context)
{
	if(event_type==NRF_TIMER_EVENT_COMPARE0)
	{
		nrf_gpio_pin_toggle(LED_1);
	}
}
																					 
int main(void)
{
	nrf_gpio_cfg_output(LED_1);
	
	nrf_drv_timer_init(&timer,&timerConfig,timer_event_handler);
	
	nrf_drv_timer_extended_compare(&timer,NRF_TIMER_CC_CHANNEL0,
	nrf_timer_ms_to_ticks(1000,TIMER0_CONFIG_FREQUENCY),
	NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,1);
	
	nrf_drv_timer_enable(&timer);
	
	while (1)
	{
		//Do nothing
	}
}
/** @} */
