#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <nrf.h>
#include <boards.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>
#include <nrf_uart.h>

void 		uart_putchar(uint8_t ch);
uint8_t uart_getchar(void);
void 		uart_puts(uint8_t * str);
void 		uart_gets(uint8_t * buf,int len);

bool 		uart_getchar_t(uint8_t *ch);					//Use Timeout
int  		uart_gets_t(uint8_t * buf,int size);	//Use Timeout

int main()
{
	uint8_t ch;
	uint8_t buffer[]="Hello world!\r\n";
	
	nrf_gpio_cfg_output(LED_1);

	nrf_uart_configure(NRF_UART0,NRF_UART_PARITY_INCLUDED,NRF_UART_HWFC_DISABLED);
	nrf_uart_baudrate_set(NRF_UART0,NRF_UART_BAUDRATE_115200);
  nrf_uart_txrx_pins_set(NRF_UART0,6,8);
	nrf_uart_enable(NRF_UART0);
	nrf_uart_task_trigger(NRF_UART0,NRF_UART_TASK_STARTTX);
	nrf_uart_task_trigger(NRF_UART0,NRF_UART_TASK_STARTRX);
	
	uart_puts(buffer);
	
	while(1)
	{		
		ch=uart_getchar();
		uart_putchar(ch);

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
/********************************************
* @brief	Function for get character from uart use timeout.
*	Default Timeout ~= 500 ms.
*	@param[out]	ch	Pointer to Character for save data.
*	@retval	bool	1 for success & 0 for timeout error.
*/
bool uart_getchar_t(uint8_t *ch)
{
	uint32_t TimeOut=0xfffff;
	while(!nrf_uart_event_check(NRF_UART0,NRF_UART_EVENT_RXDRDY)&&TimeOut)TimeOut--;
	if(!TimeOut)return 0;
	nrf_uart_event_clear(NRF_UART0,NRF_UART_EVENT_RXDRDY);
	*ch=nrf_uart_rxd_get(NRF_UART0);
	return 1;
}
/********************************************
* @brief	Function for get string from uart use timeout.
*	Default Timeout ~= 500 ms Set in uart_getchar_t function.
*	@param[out]	buf	Pointer to buffer for save string.
*	@param[in]	size	Size of buffer or max size of string to recive.
*	@retval	int	Number or charecter recived, 0 for timeout error.
*/
int uart_gets_t(uint8_t * buf,int size)
{
	uint8_t ch;
	int Conter=0;
	while(uart_getchar_t(&ch))
	{
		*buf=ch;
		buf++;
		Conter++;
		if(Conter>=size)break;
	}
	*buf='\0';
	return Conter;
}
/**
*	@}
*/
