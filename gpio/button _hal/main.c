#include <nrf.h>
#include <boards.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>

int main()
{
	nrf_gpio_cfg_output(LED_1);
	nrf_gpio_cfg_input(BUTTON_1,NRF_GPIO_PIN_PULLUP);
	while(1)
	{
		if(!nrf_gpio_pin_read(BUTTON_1))
		{
			nrf_gpio_pin_toggle(LED_1);
			while(!nrf_gpio_pin_read(BUTTON_1));
			nrf_delay_ms(50);
		}
	}
}
