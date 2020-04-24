#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <nrf.h>
#include <boards.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>
#include <nrf_clock.h>

#include "uart.h"

int main()
{
	nrf_clock_hf_src_t HClock_SRC;
	nrf_clock_lf_src_t LClock_SRC;
	
	//init uart
	uart_init();
	
	//Set HF
	nrf_clock_task_trigger(NRF_CLOCK_TASK_HFCLKSTART);
	while(!nrf_clock_event_check(NRF_CLOCK_EVENT_HFCLKSTARTED));
	nrf_clock_event_clear(NRF_CLOCK_EVENT_HFCLKSTARTED);
	
	//Check HF
	uart_puts((uint8_t *)"The HF Clock SRC is: ");
	HClock_SRC=nrf_clock_hf_src_get();
	switch(HClock_SRC)
	{
		case NRF_CLOCK_HF_SRC_LOW_ACCURACY :uart_puts((uint8_t *)"64 MHz internal RC oscillator (HFINT).\r\n");break;
		case NRF_CLOCK_HF_SRC_HIGH_ACCURACY:uart_puts((uint8_t *)"64 MHz Crystal oscillator (HFXO).\r\n");break;//XTAL
	}
	
	//Set LF
	nrf_clock_lf_src_set(NRF_CLOCK_LF_SRC_Xtal);
	nrf_clock_task_trigger(NRF_CLOCK_TASK_LFCLKSTART);
	while(!nrf_clock_event_check(NRF_CLOCK_EVENT_LFCLKSTARTED));
	nrf_clock_event_clear(NRF_CLOCK_EVENT_LFCLKSTARTED);
	
	//Check LF
	uart_puts((uint8_t *)"The LF Clock SRC is: ");
	LClock_SRC=nrf_clock_lf_src_get();
	switch(LClock_SRC)
	{
		case NRF_CLOCK_LF_SRC_RC :uart_puts((uint8_t *)"Internal 32 kHz RC oscillator.\r\n");break;
		case NRF_CLOCK_LF_SRC_Xtal:uart_puts((uint8_t *)"External 32 kHz crystal.\r\n");break;
		case NRF_CLOCK_LF_SRC_Synth:uart_puts((uint8_t *)"Internal 32 kHz synthesizer from HFCLK system clock.\r\n");break;
	}
	
	//loop
	while(1)
	{	
	}
}
/**
*	@}
*/
