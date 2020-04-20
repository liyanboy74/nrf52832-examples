#include <nrf.h>
#include <boards.h>
#include <nrf_delay.h>
#include <nrf_timer.h>
#include <nrf_gpio.h>
#include <nrf_gpiote.h>
#include <nrf_ppi.h>

int main()
{
	//Set LED as output
	nrf_gpio_cfg_output(LED_1);
	
	//Set value & Task Toggel for LED
	nrf_gpiote_task_configure(0,LED_1,NRF_GPIOTE_POLARITY_TOGGLE,NRF_GPIOTE_INITIAL_VALUE_HIGH);
	nrf_gpiote_task_enable(0);
	
	//Set Timer To generate event every 1s
	nrf_timer_bit_width_set(NRF_TIMER0,NRF_TIMER_BIT_WIDTH_32);	
	nrf_timer_mode_set(NRF_TIMER0,NRF_TIMER_MODE_TIMER);
	nrf_timer_frequency_set(NRF_TIMER0,NRF_TIMER_FREQ_1MHz);
	nrf_timer_cc_write(NRF_TIMER0,NRF_TIMER_CC_CHANNEL0,nrf_timer_ms_to_ticks(1000,NRF_TIMER_FREQ_1MHz));
	nrf_timer_shorts_enable(NRF_TIMER0,NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK);
	nrf_timer_task_trigger(NRF_TIMER0,NRF_TIMER_TASK_START);
	
	//setup ppi betwen Timer & LED
	nrf_ppi_channel_endpoint_setup(NRF_PPI_CHANNEL0,(uint32_t)&NRF_TIMER0->EVENTS_COMPARE[0],(uint32_t)&NRF_GPIOTE->TASKS_OUT[0]);
	nrf_ppi_channel_enable(NRF_PPI_CHANNEL0);
	
	while(1)
	{
	}
}
