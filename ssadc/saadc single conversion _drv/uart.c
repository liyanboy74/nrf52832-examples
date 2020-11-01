#include "app_uart.h"
#include "nrf_error.h"

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
