#include <stdint.h>
#include <boards.h>
#include <nfc_t2t_lib.h>
#include <nfc_uri_msg.h>

static const char url[] ={'r','o','n','i','k','a','p','a','r','s','.','i','r'};

uint8_t ndef_msg_buf[256];

void nfc_callback(void *context, NfcEvent event, const char *data, size_t dataLength)
{
	 switch (event)
	{
		case NFC_EVENT_FIELD_ON:
			nrf_gpio_pin_write(LED_1,0);
			break;
		
		case NFC_EVENT_FIELD_OFF:
			nrf_gpio_pin_write(LED_1,1);
			break;
		
		default:
			break;
	}
}

int main()
{
	uint32_t len;
	
	nrf_gpio_cfg_output(LED_1);
	nrf_gpio_pin_write(LED_1,1);
	
	nfcSetup(nfc_callback,NULL);
	
	len=sizeof(ndef_msg_buf);
	nfc_uri_msg_encode( NFC_URI_HTTP_WWW,(uint8_t *) url,sizeof(url),ndef_msg_buf,&len);
	
	nfcSetPayload( (char*)ndef_msg_buf, len);
	
	nfcStartEmulation();
	
	while(1)
	{
		
	}
}
