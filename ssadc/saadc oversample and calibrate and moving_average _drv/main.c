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

#define Avg_Slope		((float)(3.6/16382))	//for Gain=1/6 & Resolution=14 bit
#define Calibrate_for_Voltage				1000	//mv
#define Moving_average_Size					25

#define UART_HWFC APP_UART_FLOW_CONTROL_DISABLED

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
//------------------ Moving_average -----------------------
nrf_saadc_value_t Moving_average_Data[Moving_average_Size];
uint8_t Moving_average_Index=0;

void Moving_average_Clear(void)
{
	memset(Moving_average_Data,0,Moving_average_Size);
	Moving_average_Index=0;
	
}
float Moving_average(nrf_saadc_value_t NewData)
{
	int i,j;
	float Av=0;
	
	Moving_average_Data[Moving_average_Index]=NewData;
	
	for(i=0,j=Moving_average_Index;i<Moving_average_Size;i++,j--)
	{
		if(j<0)j=Moving_average_Size-1;
		Av+=(1.0/Moving_average_Size)*Moving_average_Data[j];
	}
	
	Moving_average_Index++;
	if(Moving_average_Index>=Moving_average_Size)Moving_average_Index=0;
	
	return Av;
}
//-----------------------------------------------------------
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
			Adc=Moving_average(Adc);
			if(Calibr==0)
			{
				sprintf((char*)Buf,"SAADC : %04d -> V = %.4f\r\n",Adc,Adc*Avg_Slope);
				uart_puts(Buf);
			}
			else
			{	
				float Out,Dif;
				
				Dif= Adc - Calibr;
				
				Out= Dif * Avg_Slope + (Calibrate_for_Voltage/1000.0f);
				
				sprintf((char*)Buf,"V = %.4f\r\n",Out);
				uart_puts(Buf);
			}

			nrf_gpio_pin_toggle(LED_1);

			nrf_delay_ms(100);
			
			if(app_uart_get(&Ch)==NRF_SUCCESS)
			{
				if(Ch=='\r')
				{
					sprintf((char*)Buf,"Change Voltage to %dmv & Press Enter\r\n",Calibrate_for_Voltage);
					uart_puts(Buf);
					nrf_delay_ms(2000);
					while(1)
					{
						nrf_drv_saadc_sample_convert(0,&Adc);
						Adc=Moving_average(Adc);
						sprintf((char*)Buf,"SAADC : %04d\r\n",Adc);
						uart_puts(Buf);
						nrf_delay_ms(100);
						
						if(app_uart_get(&Ch)==NRF_SUCCESS)
						{
							if(Ch=='\r')
							{
								Calibr=Adc;
								uart_puts((uint8_t*)"Calibr Done!\r\n");
								nrf_delay_ms(2000);
								break;
							}
						}
					}
				}
			}
		}
	}
}
