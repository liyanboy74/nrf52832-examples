#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <nrf.h>
#include <boards.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>
#include <nrf_nvmc.h>

#include "uart.h"

#define Page127				0x0007f000
#define PageSize			0x1000

int main()
{
	int i,len;
	uint8_t *p;
	uint8_t str[]="Hello from Flash!\r\n";
	
	len=sizeof(str);
	
	//uart init
	uart_init();
	
	//Eraser Page127
	nrf_nvmc_page_erase(Page127);
	
	//Write to Flash
	nrf_nvmc_write_bytes(Page127,str,len);
	
	//Read from Flash
	p=(uint8_t *)Page127;
	for(i=0;i<len;i++,p++)
	{
		uart_putchar(*p);
	}
}
/**	@} */
