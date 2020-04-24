#include <nrf.h>
#include <nrf_uart.h>

/********************************************
* @brief	Function for init uart.
*/
void uart_init(void)
{
	nrf_uart_configure(NRF_UART0,NRF_UART_PARITY_INCLUDED,NRF_UART_HWFC_DISABLED);
	nrf_uart_baudrate_set(NRF_UART0,NRF_UART_BAUDRATE_115200);
  nrf_uart_txrx_pins_set(NRF_UART0,6,8);
	nrf_uart_enable(NRF_UART0);
	nrf_uart_task_trigger(NRF_UART0,NRF_UART_TASK_STARTTX);
	nrf_uart_task_trigger(NRF_UART0,NRF_UART_TASK_STARTRX);
}
/********************************************
* @brief	Function for send character on uart.
*
*	@param[in]	ch	Character.
*/
void uart_putchar(uint8_t ch)
{
	nrf_uart_txd_set(NRF_UART0,ch);
	while(!nrf_uart_event_check(NRF_UART0,NRF_UART_EVENT_TXDRDY));
	nrf_uart_event_clear(NRF_UART0,NRF_UART_EVENT_TXDRDY);
}
/********************************************
* @brief	Function for get character from uart.
*
*	@retval	Character.
*/
uint8_t uart_getchar(void)
{
	while(!nrf_uart_event_check(NRF_UART0,NRF_UART_EVENT_RXDRDY));
	nrf_uart_event_clear(NRF_UART0,NRF_UART_EVENT_RXDRDY);
	return nrf_uart_rxd_get(NRF_UART0);
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
