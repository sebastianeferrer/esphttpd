#include <esp8266.h>
#include "attenuatorController.h"

#define CHANNELS    		16
//pin states
#define LOW          		0
#define HIGH      			1
//GPIOS NUMBERS
#define LATCH_PIN_NUMBER 	1
#define DATA_PIN_NUMBER    	3
#define CLOCK_PIN_NUMBER   	0
#define OE_PIN_NUMBER      	2
//PIN MUX REGs
#define LATCH_PIN   		PERIPHS_IO_MUX_U0TXD_U
#define DATA_PIN    		PERIPHS_IO_MUX_U0RXD_U
#define CLOCK_PIN   		PERIPHS_IO_MUX_GPIO0_U
#define OE_PIN      		PERIPHS_IO_MUX_GPIO2_U
//GPIOS FUNTIONS
#define LATCH_PIN_FUNC 		FUNC_GPIO1
#define DATA_PIN_FUNC  		FUNC_GPIO3
#define CLOCK_PIN_FUNC 		FUNC_GPIO0
#define OE_PIN_FUNC    		FUNC_GPIO2
//GENERAL
#define DELAY_US    		1000

unsigned int secuences[26]={
5416,
5672,
6440,
6696,
9512,
9768,
10536,
10792,
5424,
5680,
5448,
5704,
6472,
6728,
9544,
9800,
10568,
10824,
5456,
5712,
6480,
6736,
9552,
9808,
10576,
10832 
};

static char attenuation_db=0;
static char attenuation_string[]="00";

//Cgi that controlls attenuation according to the 'attenuation_db' param in the POST data
int ICACHE_FLASH_ATTR cgiAttenuationController(HttpdConnData *connData) {
	int len;
	char buff[1024];
	
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->post->buff, "attenuation_db", buff, sizeof(buff));
	if (len!=0) {
		attenuation_db=atoi(buff);
		switch(attenuatorController(attenuation_db)){
			case RV_SUCCESS:
				os_strcpy(attenuation_string, buff);
				os_printf("Attenuator setted at %d dB\n", attenuation_db);
				break;
			case RV_ILLEGAL:
			default:
				os_printf("\nAttenuator Controller: attenuation higher than possible.\n");
				break;
		}
	}

	httpdRedirect(connData, "attenuatorController.html");
	return HTTPD_CGI_DONE;
}

//Template code for the attenuationController page.
int ICACHE_FLASH_ATTR htmlAttenuationController(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return HTTPD_CGI_DONE;

	os_strcpy(buff, "Unknown");
	if (os_strcmp(token, "attenuation_string")==0) {
		os_strcpy(buff, attenuation_string);
	}
	httpdSend(connData, buff, -1);
	return HTTPD_CGI_DONE;
}

retval_t attenuatorController(unsigned int attenuation_db){
	unsigned int status=0;
	unsigned int i=0;
	unsigned int secuence=secuences[0];
	secuence=secuences[attenuation_db];
	if(attenuation_db>(sizeof(secuences)/sizeof(secuences[0]))){
		return RV_ILLEGAL;
	}
	else{
		os_printf("\nAttenuator Controller:\n");
		for(i=0;i<CHANNELS;i++){
			status=secuence%2;
			secuence=secuence/2;

			if(status==0)
			{
				GPIO_OUTPUT_SET(DATA_PIN_NUMBER, LOW);
				os_printf("0");
			}
			else if(status==1)
			{
				GPIO_OUTPUT_SET(DATA_PIN_NUMBER, HIGH);
				os_printf("1");
			}
			GPIO_OUTPUT_SET(CLOCK_PIN_NUMBER, HIGH);
			os_delay_us(DELAY_US);
			GPIO_OUTPUT_SET(CLOCK_PIN_NUMBER, LOW);
			os_delay_us(DELAY_US);      
		}
		GPIO_OUTPUT_SET(LATCH_PIN_NUMBER, HIGH);
		os_delay_us(DELAY_US);
		GPIO_OUTPUT_SET(LATCH_PIN_NUMBER, LOW);
		os_delay_us(DELAY_US);
		os_printf("\n END OF PULSE TRAIN.\n");
		return RV_SUCCESS;
	}
}

void attenuatorInit(void) {
	PIN_FUNC_SELECT(DATA_PIN, DATA_PIN_FUNC);
	gpio_output_set(0/*set_mask*/, 0/*clear_mask*/, (1<<DATA_PIN_NUMBER)/*enable_mask*/, 0/*disable_mask*/);
	PIN_FUNC_SELECT(LATCH_PIN, LATCH_PIN_FUNC);
	gpio_output_set(0/*set_mask*/, 0/*clear_mask*/, (1<<LATCH_PIN_NUMBER)/*enable_mask*/, 0/*disable_mask*/);
	PIN_FUNC_SELECT(CLOCK_PIN, CLOCK_PIN_FUNC);
	gpio_output_set(0/*set_mask*/, 0/*clear_mask*/, (1<<CLOCK_PIN_NUMBER)/*enable_mask*/, 0/*disable_mask*/);
	PIN_FUNC_SELECT(OE_PIN, OE_PIN_FUNC);
	gpio_output_set(0/*set_mask*/, 0/*clear_mask*/, (1<<OE_PIN_NUMBER)/*enable_mask*/, 0/*disable_mask*/);
	
	GPIO_OUTPUT_SET(OE_PIN_NUMBER, LOW);
	attenuatorController(0);
}
