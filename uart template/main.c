#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <nrf.h>
#include <boards.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>

#include "uart.h"

int main()
{
	uint8_t ch;
	uint8_t buffer[]="Hello world!\r\n";
	
	nrf_gpio_cfg_output(LED_1);

	uart_init();
	
	uart_puts(buffer);
	
	while(1)
	{		
		ch=uart_getchar();
		uart_putchar(ch);

		nrf_gpio_pin_toggle(LED_1);
	}
}
/**
*	@}
*/
