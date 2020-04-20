#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <nrf.h>
#include <boards.h>
#include <nrf_clock.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>
#include <nrf_saadc.h>

#include "uart.h"

int main()
{
	int i=0;
	
	float precise_result[10];
	nrf_saadc_value_t result[10];
	nrf_saadc_channel_config_t saadcConf;
	
	uint8_t buffer[32];
	
	nrf_gpio_cfg_output(LED_1);
	uart_init();
	
	// Start HFCLK from crystal oscillator, this will give the SAADC higher accuracy
	nrf_clock_task_trigger(NRF_CLOCK_TASK_HFCLKSTART);
	while(!nrf_clock_event_check(NRF_CLOCK_EVENT_HFCLKSTARTED));
	nrf_clock_event_clear(NRF_CLOCK_EVENT_HFCLKSTARTED);
	
	// Configure SAADC singled-ended channel, Internal reference (0.6V) and 1/6 gain.
	// Configure the SAADC channel with VDD as positive input, no negative input(single ended).
	saadcConf.gain=NRF_SAADC_GAIN1_6;
	saadcConf.mode=NRF_SAADC_MODE_SINGLE_ENDED;
	saadcConf.reference=NRF_SAADC_REFERENCE_INTERNAL;
	saadcConf.resistor_p=NRF_SAADC_RESISTOR_DISABLED;
	saadcConf.resistor_n=NRF_SAADC_RESISTOR_DISABLED;
	saadcConf.acq_time=NRF_SAADC_ACQTIME_40US;
	saadcConf.pin_p=NRF_SAADC_INPUT_VDD;
	saadcConf.pin_n=NRF_SAADC_INPUT_DISABLED;
	nrf_saadc_channel_init(0,&saadcConf);
	
	// Configure the SAADC resolution.
	nrf_saadc_resolution_set(NRF_SAADC_RESOLUTION_14BIT);
	
	// Configure result to be put in RAM at the location of "result" variable.
	nrf_saadc_buffer_init(&result[0],10);

	// Enable SAADC
	nrf_saadc_enable();
	while(!nrf_saadc_enable_check());
	
	// Calibrate the SAADC (only needs to be done once in a while)
	nrf_saadc_task_trigger(NRF_SAADC_TASK_CALIBRATEOFFSET);
	while(!nrf_saadc_event_check(NRF_SAADC_EVENT_CALIBRATEDONE));
	nrf_saadc_event_clear(NRF_SAADC_EVENT_CALIBRATEDONE);
	while(nrf_saadc_busy_check());

	while(1)
	{
		nrf_saadc_task_trigger(NRF_SAADC_TASK_START);
		while(!nrf_saadc_event_check(NRF_SAADC_EVENT_STARTED));
		nrf_saadc_event_clear(NRF_SAADC_EVENT_STARTED);

		for(i=0;i<10;i++)
		{
			nrf_saadc_task_trigger(NRF_SAADC_TASK_SAMPLE);
			nrf_delay_ms(1);
		}
		
		while(!nrf_saadc_event_check(NRF_SAADC_EVENT_END));
		nrf_saadc_event_clear(NRF_SAADC_EVENT_END);
		
		nrf_saadc_task_trigger(NRF_SAADC_TASK_STOP);
		while(!nrf_saadc_event_check(NRF_SAADC_EVENT_STOPPED));
		nrf_saadc_event_clear(NRF_SAADC_EVENT_STOPPED);
		
		for(i=0;i<10;i++)
		{
			// Convert the result to voltage
			// Result = [V(p) - V(n)] * GAIN/REFERENCE * 2^(RESOLUTION)
			// Result = (VDD - 0) * ((1/6) / 0.6) * 2^14
			// VDD = Result / 4551.1
			precise_result[i] = (float)result[i] / 4551.1f;
			sprintf((char *)buffer,"%.2f  ",precise_result[i]);
			uart_puts(buffer);
		}
		uart_puts((uint8_t *)"\r\n");
		
		nrf_gpio_pin_toggle(LED_1);
		nrf_delay_ms(500);
	}
}
/**
*	@}
*/
