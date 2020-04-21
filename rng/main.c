#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <nrf.h>
#include <boards.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>
#include <nrf_rng.h>

#include "uart.h"

int main()
{
	uint8_t buffer[16];
	uint8_t rng_value;
	
	//uart init
	uart_init();
	
	//RNG start
	nrf_rng_task_trigger(NRF_RNG_TASK_START);
	
	while(1)
	{
		//get random value
		while(!nrf_rng_event_get(NRF_RNG_EVENT_VALRDY));
		nrf_rng_event_clear(NRF_RNG_EVENT_VALRDY);
		rng_value=nrf_rng_random_value_get();
		
		//print value
		sprintf((char *)buffer,"%3d\t",rng_value);
		uart_puts(buffer);
		
		//delay
		nrf_delay_ms(200);
	}
}
/** @} */
