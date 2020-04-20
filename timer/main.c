#include <nrf.h>
#include <boards.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>
#include <nrf_timer.h>

int main()
{
	nrf_gpio_cfg_output(LED_4);

	nrf_timer_bit_width_set(NRF_TIMER0,NRF_TIMER_BIT_WIDTH_32);
	nrf_timer_frequency_set(NRF_TIMER0,NRF_TIMER_FREQ_1MHz);
	nrf_timer_cc_write(NRF_TIMER0,NRF_TIMER_CC_CHANNEL0,nrf_timer_ms_to_ticks(1000,NRF_TIMER_FREQ_1MHz));
	nrf_timer_shorts_enable(NRF_TIMER0,TIMER_SHORTS_COMPARE0_CLEAR_Msk);
	nrf_timer_int_enable(NRF_TIMER0,NRF_TIMER_INT_COMPARE0_MASK);
	
	nrf_timer_task_trigger(NRF_TIMER0,NRF_TIMER_TASK_START);
	
	NVIC_EnableIRQ(TIMER0_IRQn);
	
	while(1)
	{

	}
}
void TIMER0_IRQHandler(void)
{
	if(nrf_timer_event_check(NRF_TIMER0,NRF_TIMER_EVENT_COMPARE0))
	{
		nrf_timer_event_clear(NRF_TIMER0,NRF_TIMER_EVENT_COMPARE0);
		nrf_gpio_pin_toggle(LED_4);
	}
}
