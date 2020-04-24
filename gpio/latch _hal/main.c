#include <nrf.h>
#include <stdbool.h>
#include <bsp.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>
																			 
int main(void)
{
	//Config LED as Out and Set that.
	nrf_gpio_cfg_output(LED_1);
	nrf_gpio_pin_write(LED_1,1);
	
	//Config Button as Input,PullUp,SenseLow
	nrf_gpio_cfg(BUTTON_1,NRF_GPIO_PIN_DIR_INPUT,NRF_GPIO_PIN_INPUT_CONNECT,
	NRF_GPIO_PIN_PULLUP,NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_SENSE_LOW);

	while (1)
	{
		//Read Latch
		if((NRF_GPIO->LATCH>>BUTTON_1)&1)
		{
			//Clear Latch
			NRF_GPIO->LATCH|=1<<BUTTON_1;
			
			//Toggle LED
			nrf_gpio_pin_toggle(LED_1);
		}
		//Delay 1S
		nrf_delay_ms(2000);
	}
}
/** @} */
