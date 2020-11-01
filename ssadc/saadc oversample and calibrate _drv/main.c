#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nrf.h"
#include "boards.h"
#include "nrf_delay.h"

#include "app_uart.h"
#include "app_error.h"

#include "nrf_drv_saadc.h"

#include "uart.h"

#define UART_TX_BUF_SIZE 128                        /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 64                         /**< UART RX buffer size. */

#define UART_HWFC APP_UART_FLOW_CONTROL_DISABLED

#define Avg_Slope		((float)(3.6/16382))	//for 14 bit

void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}

void saadc_event_handle(nrf_drv_saadc_evt_t const * p_event)
{
	if(p_event->type==NRF_DRV_SAADC_EVT_DONE)
	{
		
	}
}

void Config_SAADC_BURST(uint8_t Channel,uint8_t Enable_Disable)
{
	if(Enable_Disable)
		NRF_SAADC->CH[Channel].CONFIG|= SAADC_CH_CONFIG_BURST_Msk;
	else
		NRF_SAADC->CH[Channel].CONFIG&=~SAADC_CH_CONFIG_BURST_Msk;
}

int main()
{
	nrf_saadc_value_t Adc,Calibr=0;
	uint8_t Buf[64],Ch;
	uint32_t err_code;

	const app_uart_comm_params_t comm_params =
	{
			RX_PIN_NUMBER,
			TX_PIN_NUMBER,
			RTS_PIN_NUMBER,
			CTS_PIN_NUMBER,
			UART_HWFC,
			false,
			UART_BAUDRATE_BAUDRATE_Baud115200
	};
	
	APP_UART_FIFO_INIT(&comm_params,
		UART_RX_BUF_SIZE,
		UART_TX_BUF_SIZE,
		uart_error_handle,
		APP_IRQ_PRIORITY_LOW,
		err_code);

	APP_ERROR_CHECK(err_code);
	
	const nrf_drv_saadc_config_t SaadcConfig=NRF_DRV_SAADC_DEFAULT_CONFIG;

	nrf_saadc_channel_config_t SaadcChConfig=
		NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN1);
	
	SaadcChConfig.acq_time=NRF_SAADC_ACQTIME_40US;
	SaadcChConfig.gain=NRF_SAADC_GAIN1_6;
	
	nrf_drv_saadc_init(&SaadcConfig,saadc_event_handle);
	
	nrf_drv_saadc_channel_init(0,&SaadcChConfig);
	
	Config_SAADC_BURST(0,1);
	
	nrf_gpio_cfg_output(LED_1);
	
	while(1)
	{
		err_code=nrf_drv_saadc_sample_convert(0,&Adc);
		
		if(err_code==NRF_SUCCESS)
		{
			if(Calibr==0)
			{
				sprintf((char*)Buf,"SAADC : %04d -> V = %.4f\r\n",Adc,Adc*Avg_Slope);
				uart_puts(Buf);
			}
			else
			{	
				float Out,Dif;
				
				Dif= Adc - Calibr;
				
				Out= Dif * Avg_Slope + 1.0f;
				
				sprintf((char*)Buf,"V = %.4f\r\n",Out);
				uart_puts(Buf);
			}

			nrf_gpio_pin_toggle(LED_1);

			nrf_delay_ms(500);
			
			if(app_uart_get(&Ch)==NRF_SUCCESS)
			{
				if(Ch=='\r')
				{
					uart_puts((uint8_t*)"Change Voltage to 1000mv & Press Enter\r\n");
					while(1)
					{
						nrf_drv_saadc_sample_convert(0,&Adc);
						sprintf((char*)Buf,"SAADC : %04d\r\n",Adc);
						uart_puts(Buf);
						nrf_delay_ms(500);
						
						if(app_uart_get(&Ch)==NRF_SUCCESS)
						{
							if(Ch=='\r')
							{
								Calibr=Adc;
								uart_puts((uint8_t*)"Calibr Done!\r\n");
								break;
							}
						}
					}
				}
			}
		}
	}
}
