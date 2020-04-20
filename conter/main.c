#include <nrf.h>
#include <boards.h>
#include <nrf_delay.h>
#include <nrf_timer.h>
#include <nrf_gpio.h>
#include <nrf_gpiote.h>
#include <nrf_ppi.h>
#include "uart.h"

uint8_t buf[32];

int main()
{
	
	//bull up button
	nrf_gpio_cfg_input(BUTTON_1,NRF_GPIO_PIN_PULLUP);
	
	//Set event for press button
	nrf_gpiote_event_configure(0,BUTTON_1,NRF_GPIOTE_POLARITY_HITOLO);
	nrf_gpiote_event_enable(0);
	
	//Enable intrupt for event to show value of conter
	nrf_gpiote_int_enable(NRF_GPIOTE_INT_IN0_MASK);
	
	//setup timer as conter
	nrf_timer_bit_width_set(NRF_TIMER0,NRF_TIMER_BIT_WIDTH_32);	
	nrf_timer_mode_set(NRF_TIMER0,NRF_TIMER_MODE_COUNTER);
	nrf_timer_task_trigger(NRF_TIMER0,NRF_TIMER_TASK_START);
	
	//setup ppi betwen button & Conter
	nrf_ppi_channel_endpoint_setup(NRF_PPI_CHANNEL0,(uint32_t)&NRF_GPIOTE->EVENTS_IN[0],(uint32_t)&NRF_TIMER0->TASKS_COUNT);
	nrf_ppi_channel_enable(NRF_PPI_CHANNEL0);
	
	//init uart
	uart_init();
	
	//enable intrupt
	NVIC_EnableIRQ(GPIOTE_IRQn);
	
	while(1)
	{
		__WFE;
	}
}
void GPIOTE_IRQHandler()
{
	if(NRF_GPIOTE->EVENTS_IN[0])
	{
		nrf_gpiote_event_clear(NRF_GPIOTE_EVENTS_IN_0);
		//Capture conter
		nrf_timer_task_trigger(NRF_TIMER0,NRF_TIMER_TASK_CAPTURE1);
		sprintf((char *)buf,"%d\r\n",nrf_timer_cc_read(NRF_TIMER0,NRF_TIMER_CC_CHANNEL1));
		uart_puts(buf);
	}
}
