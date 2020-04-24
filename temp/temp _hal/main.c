#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <nrf.h>
#include <boards.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>
#include <nrf_clock.h>
#include <nrf_temp.h>

#include "uart.h"

int main()
{
	float temp;
	uint8_t buffer[64];
	
	//init uart
	uart_init();
	
	//Set HF
	nrf_clock_task_trigger(NRF_CLOCK_TASK_HFCLKSTART);
	while(!nrf_clock_event_check(NRF_CLOCK_EVENT_HFCLKSTARTED));
	nrf_clock_event_clear(NRF_CLOCK_EVENT_HFCLKSTARTED);
	
	nrf_temp_init();
	
	//loop
	while(1)
	{
		NRF_TEMP->TASKS_START=1;
		while (!NRF_TEMP->EVENTS_DATARDY);
		NRF_TEMP->EVENTS_DATARDY = 0;
		
		temp=nrf_temp_read()/(float)4;
		
		sprintf((char *)buffer,"Temp=%.2f\r\n",temp);
		uart_puts(buffer);
		
		nrf_delay_ms(1000);
	}
}
/**
*	@}
*/
