#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <nrf.h>
#include <boards.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>
#include <nrf_uart.h>
#include <app_uart.h>
#include <app_error.h>

#define UART_TX_BUF_SIZE 64
#define UART_RX_BUF_SIZE 64

uint8_t uart_rx_buffer[UART_RX_BUF_SIZE];
uint8_t uart_tx_buffer[UART_TX_BUF_SIZE];

void 		uart_putchar(uint8_t ch);
uint8_t uart_getchar(void);
void 		uart_puts(uint8_t * str);
void 		uart_gets(uint8_t * buf,int len);

void uart_event_handler (app_uart_evt_t * p_app_uart_event)
{
	if (p_app_uart_event->evt_type == APP_UART_COMMUNICATION_ERROR)
	{
		APP_ERROR_HANDLER(p_app_uart_event->data.error_communication);
	}
	else if (p_app_uart_event->evt_type == APP_UART_FIFO_ERROR)
	{
		APP_ERROR_HANDLER(p_app_uart_event->data.error_code);
	}
}

int main()
{
	uint8_t ch;
	uint8_t buffer[]="Hello world!\r\n";

	nrf_gpio_cfg_output(LED_1);

	app_uart_buffers_t uart_buf;
	uart_buf.rx_buf=uart_rx_buffer;
	uart_buf.rx_buf_size=UART_RX_BUF_SIZE;
	uart_buf.tx_buf=uart_tx_buffer;
	uart_buf.tx_buf_size=UART_TX_BUF_SIZE;

	app_uart_comm_params_t uartCP;
	uartCP.baud_rate=NRF_UART_BAUDRATE_115200;
	uartCP.flow_control=APP_UART_FLOW_CONTROL_DISABLED;
	uartCP.tx_pin_no=6;
	uartCP.rx_pin_no=8;
	uartCP.use_parity=false;

	app_uart_init(&uartCP,&uart_buf,uart_event_handler,APP_IRQ_PRIORITY_LOW);

	uart_puts(buffer);

	while(1)
	{
		ch=uart_getchar();
		uart_putchar(ch);

		nrf_delay_ms(10);
		nrf_gpio_pin_toggle(LED_1);
	}
}
/********************************************
* @brief	Function for send character on uart.
*
*	@param[in]	ch	Character.
*/
void uart_putchar(uint8_t ch)
{
	while (app_uart_put(ch) != NRF_SUCCESS);
}
/********************************************
* @brief	Function for get character from uart.
*
*	@retval	Character.
*/
uint8_t uart_getchar(void)
{
	uint8_t ch;
	while(app_uart_get(&ch)!=NRF_SUCCESS);
	return ch;
}
/********************************************
* @brief	Function for send string to uart.
*
*	@param[in]	str	Pointer to string.
*/
void uart_puts(uint8_t * str)
{
	while(*str!='\0')
	{
		uart_putchar(*str);
		str++;
	}
}
/********************************************
* @brief	Function for get string from uart.
*
*	@param[out]	buf	Pointer to buffer for save string.
*	@param[in]	len Length of string to recive.
*/
void uart_gets(uint8_t * buf,int len)
{
	while(len)
	{
		*buf=uart_getchar();
		len--;
		buf++;
	}
	*buf='\0';
}
/**
*	@}
*/
