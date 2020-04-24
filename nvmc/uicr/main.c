#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <nrf.h>
#include <boards.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>
#include <nrf_nvmc.h>

#include "uart.h"

#define UICR_BaseAddress			((uint32_t)0x10001000)
#define UICR_CustomerOffset		((uint32_t)0x080)
#define UICR_CustomerSize			((uint32_t)0x20)
#define UICR_CustomerAddress	((uint32_t)UICR_BaseAddress+UICR_CustomerOffset)

void NVMC_Ready()
{
	while (NRF_NVMC->READY == NVMC_READY_READY_Busy);
}
void UICR_Erase()
{
	NRF_NVMC->CONFIG=NVMC_CONFIG_WEN_Een;
	NVMC_Ready();
	NRF_NVMC->ERASEUICR=NVMC_ERASEUICR_ERASEUICR_Erase;
	NVMC_Ready();
	NRF_NVMC->CONFIG=NVMC_CONFIG_WEN_Ren;
	NVMC_Ready();
}

int main()
{
	int i,len;
	uint8_t *p;
	uint8_t str[]="Hello from UICR!\r\n";
	
	len=sizeof(str);
	
	//uart init
	uart_init();
	
	//Eraser UICR
	UICR_Erase();
	
	//Write to UICR
	nrf_nvmc_write_bytes(UICR_CustomerAddress,str,len);
	
	//Read from UICR
	p=(uint8_t *)UICR_CustomerAddress;
	
	for(i=0;i<len;i++,p++)
	{
		uart_putchar(*p);
	}
	
	//Loop
	while(1)
	{
	}
}
/**	@} */
