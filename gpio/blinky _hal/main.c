#include <nrf.h>
#include <boards.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>

int main()
{
	nrf_gpio_cfg_output(LED_1);
	while(1)
	{
		nrf_gpio_pin_toggle(LED_1);
		nrf_delay_ms(500);
	}
}
