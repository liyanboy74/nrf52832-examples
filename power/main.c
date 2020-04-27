#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <nrf.h>
#include <boards.h>
#include <nrf_gpio.h>
#include <nrf_gpiote.h>
#include <nrf_delay.h>

#include "uart.h"
#include "power.h"

//Struct for save data on RAM
typedef struct{
	uint8_t D1;
	uint8_t D2;
	uint8_t Other[64];
} data_s;
data_s * Data;

int main()
{
	uint8_t i=0;
	uint8_t buff[32];
	ResetReason_e Reset;
	
	//Set Address For Data pointer
	Data=(data_s *)RAM7BaseAddress;
	
	//Enable Button_1 for Detect Signal
	nrf_gpio_cfg(BUTTON_1,NRF_GPIO_PIN_DIR_INPUT,NRF_GPIO_PIN_INPUT_CONNECT,NRF_GPIO_PIN_PULLUP,
	NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_SENSE_LOW);
	
	//Jast a button_2 fo add data
	nrf_gpio_cfg(BUTTON_2,NRF_GPIO_PIN_DIR_INPUT,NRF_GPIO_PIN_INPUT_CONNECT,NRF_GPIO_PIN_PULLUP,
	NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_NOSENSE);
	
	//Config Gpiote & Enable Intrupt for WFI
	nrf_gpio_cfg_input(BUTTON_3,NRF_GPIO_PIN_PULLUP);
	nrf_gpiote_event_configure(0,BUTTON_3,NRF_GPIOTE_POLARITY_HITOLO);
	nrf_gpiote_event_enable(0);
	nrf_gpiote_int_enable(GPIOTE_INTENSET_IN0_Enabled);
	
	//Enable GPIOTE Intrupt
	NVIC_EnableIRQ(GPIOTE_IRQn);	
	
	//uart init
	uart_init();
	
	//Enable DC/DC Regulator
	EnableDCDC();
	
	//Set RAM7 as Retention
	EnableRamRetention(7);
	
	//Get Reset Reason
	Reset=CheckResetReason();

	//Show Reset Reason on uart
	uart_puts((uint8_t *)"Reset :");
	uart_putchar('0'+Reset);
	uart_puts((uint8_t *)"\r\n");
	
	//Reset RAM Data if Power ON
	if(Reset==Reset_PowerON)
	{
		//EraseRAM(7);
		memset(Data,0,sizeof(data_s));
	}
	
	//loop
	while(1)
	{
		i++;
		uart_putchar('.');
		//Plus & Print Ram varible
		if(nrf_gpio_pin_read(BUTTON_2)==0)
		{
			Data->D1++;
			sprintf((char *)buff,"[%d]",Data->D1);
			uart_puts(buff);
		}
		//WFI
		if(i==10)
		{
			uart_puts((uint8_t *)"\r\nWFI\r\n");
			__WFI();
			uart_puts((uint8_t *)"Intrupt!\r\n");
		}
		//Power off
		if(i==20)
		{
			uart_puts((uint8_t *)"\r\nPower OFF\r\n");
			SystemOFF();
		}
		//Delay 0.5 S
		nrf_delay_ms(500);
	}
}
//Gpiote Intrupt Handler
void GPIOTE_IRQHandler()
{
	if(NRF_GPIOTE->EVENTS_IN[0])
	{
		nrf_gpiote_event_clear(NRF_GPIOTE_EVENTS_IN_0);
	}
}

/**
*	@}
*/
