#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <nrf.h>
#include <boards.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>
#include <nrf_clock.h>
#include <nrf_rtc.h>

#include "uart.h"

#define RTC0_Prescaler	0xffff 	//4095
#define RTC0_CC0_Period	8				//Tick=8Hz -> 1s= 8*Tick

#define SET 	1
#define RESET 0

bool STick=SET;

uint8_t dayInM[]={31,31,31,31,31,31,30,30,30,30,30,29};

typedef struct {
	uint8_t H;
	uint8_t M;
	uint8_t S;
}time_s;
time_s Time;

typedef struct {
	uint8_t  D;
	uint8_t  M;
	uint16_t Y;
}date_s;
date_s Date;

void RTC0_IRQHandler(void)
{
	if (NRF_RTC0->EVENTS_COMPARE[0])
  {
		uint32_t Dif,NexC0=RTC0_CC0_Period;
		nrf_rtc_event_clear(NRF_RTC0,NRF_RTC_EVENT_COMPARE_0);
		if((NRF_RTC0->COUNTER + NexC0) > 0xffffff)Dif= NexC0-(0xffffff-NRF_RTC0->COUNTER);
		else Dif =NRF_RTC0->COUNTER + NexC0;
		nrf_rtc_cc_set(NRF_RTC0,0,Dif);
		
		Time.S++;
		if(Time.S>59)
		{
			Time.S=0;
			Time.M++;
			if(Time.M>59)
			{
				Time.M=0;
				Time.H++;
				if(Time.H>23)
				{
					Time.H=0;
					Date.D++;
					if(Date.D>dayInM[Date.M-1])
					{
						Date.D=1;
						Date.M++;
						if(Date.M>12)
						{
							Date.M=1;
							Date.Y++;
						}
					}
				}
			}
		}
		STick=SET;
  }
}

int main()
{
	uint8_t buffer[64];
	nrf_gpio_cfg_output(LED_1);	
	
	Time.H=0;
	Time.M=0;
	Time.S=0;
	
	Date.Y=0;
	Date.M=1;
	Date.D=1;
	
	nrf_clock_lf_src_set(NRF_CLOCK_LF_SRC_Xtal);
	nrf_clock_task_trigger(NRF_CLOCK_TASK_LFCLKSTART);
	while(!nrf_clock_event_check(NRF_CLOCK_EVENT_LFCLKSTARTED));
	nrf_clock_event_clear(NRF_CLOCK_EVENT_LFCLKSTARTED);
	
	nrf_rtc_prescaler_set(NRF_RTC0,RTC0_Prescaler);
	nrf_rtc_cc_set(NRF_RTC0,0,RTC0_CC0_Period);
	nrf_rtc_int_enable(NRF_RTC0,NRF_RTC_INT_COMPARE0_MASK);

	NVIC_EnableIRQ(RTC0_IRQn);
	nrf_rtc_task_trigger(NRF_RTC0,NRF_RTC_TASK_START);
	
	uart_init();
	
	while(1)
	{	
		if(STick)
		{
			STick=RESET;
			nrf_gpio_pin_toggle(LED_1);
			sprintf((char *)buffer,"%04d-%02d-%02d %02d:%02d:%02d\r\n",Date.Y,Date.M,Date.D,Time.H,Time.M,Time.S);
			uart_puts(buffer);
		}
	}
}
/**
*	@}
*/
