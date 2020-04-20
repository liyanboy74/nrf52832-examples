#include <nrf.h>
#include <nrf_uart.h>

/**
* @brief	Function for init uart.
*/
void uart_init(void);

/**
* @brief	Function for send character on uart.
*
*	@param[in]	ch	Character.
*/
void uart_putchar(uint8_t ch);

/**
* @brief	Function for get character from uart.
*
*	@retval	Character.
*/
uint8_t uart_getchar(void);
/**
* @brief	Function for send string to uart.
*
*	@param[in]	str	Pointer to string.
*/
void uart_puts(uint8_t * str);

/**
* @brief	Function for get string from uart.
*
*	@param[out]	buf	Pointer to buffer for save string.
*	@param[in]	len Length of string to recive.
*/
void uart_gets(uint8_t * buf,int len);

/**
*	@}
*/
