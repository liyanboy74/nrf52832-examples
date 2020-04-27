#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <nrf.h>
#include <boards.h>
#include <nrf_gpio.h>
#include <nrf_gpiote.h>
#include <nrf_delay.h>

int main()
{
	//Set Led as Output
	nrf_gpio_cfg_output(LED_1);
	
	//Config Button
	nrf_gpio_cfg_input(BUTTON_1,NRF_GPIO_PIN_PULLUP);
	
	//Config GPIOTE
	nrf_gpiote_event_configure(0,BUTTON_1,NRF_GPIOTE_POLARITY_HITOLO);
	
	//Enable GPIOTE
	nrf_gpiote_event_enable(0);
	
	//Enable Intrupt Intrupt
	nrf_gpiote_int_enable(GPIOTE_INTENSET_IN0_Enabled);
	
	//Enable NVIC GPIOTE Intrupt
	NVIC_EnableIRQ(GPIOTE_IRQn);	
	
	//loop
	while(1)
	{
		__WFI();
	}
}
//
void GPIOTE_IRQHandler()
{
	if(NRF_GPIOTE->EVENTS_IN[0])
	{
		nrf_gpiote_event_clear(NRF_GPIOTE_EVENTS_IN_0);
		//Toggle LED
		nrf_gpio_pin_toggle(LED_1);
	}
}

/**
*	@}
*/
