#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <nrf.h>
#include <boards.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>
#include <nrf_uarte.h>

void 		uarte_putchar(uint8_t ch);
uint8_t	uarte_getchar(void);
void 		uarte_puts(uint8_t * str);
void 		uarte_gets(uint8_t * buf,int len);

int main()
{
	uint8_t ch;
	uint8_t buffer[]="Hello world!\r\n";
	
	nrf_gpio_cfg_output(LED_1);

	nrf_uarte_configure(NRF_UARTE0,NRF_UARTE_PARITY_INCLUDED,NRF_UARTE_HWFC_DISABLED);
	nrf_uarte_baudrate_set(NRF_UARTE0,NRF_UARTE_BAUDRATE_115200);
	nrf_uarte_txrx_pins_set(NRF_UARTE0,6,8);
	nrf_uarte_enable(NRF_UARTE0);
		
	uarte_puts(buffer);
	
	while(1)
	{
		ch=uarte_getchar();
		uarte_putchar(ch);
		
		nrf_gpio_pin_toggle(LED_1);
	}

}
/********************************************
* @brief	Function for send character on uarte.
*
*	@param[in]	ch	Character.
*/
void uarte_putchar(uint8_t ch)
{
	nrf_uarte_tx_buffer_set(NRF_UARTE0,&ch,1);
	nrf_uarte_task_trigger(NRF_UARTE0,NRF_UARTE_TASK_STARTTX);
	while(!nrf_uarte_event_check(NRF_UARTE0,NRF_UARTE_EVENT_ENDTX));
	nrf_uarte_event_clear(NRF_UARTE0,NRF_UARTE_EVENT_ENDTX);
}
/********************************************
* @brief	Function for get character from uarte.
*
*	@retval	Character.
*/
uint8_t uarte_getchar(void)
{
	uint8_t ch;
	nrf_uarte_rx_buffer_set(NRF_UARTE0,&ch,1);
	nrf_uarte_task_trigger(NRF_UARTE0,NRF_UARTE_TASK_STARTRX);
	while(!nrf_uarte_event_check(NRF_UARTE0,NRF_UARTE_EVENT_ENDRX));
	nrf_uarte_event_clear(NRF_UARTE0,NRF_UARTE_EVENT_ENDRX);
	return ch;
}
/********************************************
* @brief	Function for send string to uarte.
*
*	@param[in]	str	Pointer to string.
*/
void uarte_puts(uint8_t * str)
{
	nrf_uarte_tx_buffer_set(NRF_UARTE0,str,strlen((char *)str));
	nrf_uarte_task_trigger(NRF_UARTE0,NRF_UARTE_TASK_STARTTX);
	while(!nrf_uarte_event_check(NRF_UARTE0,NRF_UARTE_EVENT_ENDTX));
	nrf_uarte_event_clear(NRF_UARTE0,NRF_UARTE_EVENT_ENDTX);
}
/********************************************
* @brief	Function for get string from uarte.
*
*	@param[out]	buf	Pointer to buffer for save string.
*	@param[in]	len Length of string to recive.
*/
void uarte_gets(uint8_t * buf,int len)
{	
	nrf_uarte_rx_buffer_set(NRF_UARTE0,buf,len);
	nrf_uarte_task_trigger(NRF_UARTE0,NRF_UARTE_TASK_STARTRX);
	while(!nrf_uarte_event_check(NRF_UARTE0,NRF_UARTE_EVENT_ENDRX));
	nrf_uarte_event_clear(NRF_UARTE0,NRF_UARTE_EVENT_ENDRX);
	buf[len]='\0';
}
/**
*	@}
*/
