#include "nrf.h"
#include "boards.h"
#include "nrf_delay.h"

#include "app_uart.h"
#include "app_error.h"

#include "uart.h"

#define UART_TX_BUF_SIZE 64                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 64                         /**< UART RX buffer size. */

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

int main()
{
	uint32_t err_code;
	uint8_t Ch;
	
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
	
	nrf_gpio_cfg_output(LED_1);
	
	while(1)
	{
		Ch=uart_getchar();
		uart_putchar(Ch);
		nrf_gpio_pin_toggle(LED_1);
	}
}
