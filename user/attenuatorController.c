#include <esp8266.h>
#include "attenuatorController.h"
#include "io.h"

#define CHANNELS    	16
//pin states
#define LOW          	0
#define HIGH      	1
//GPIOS NUMBERS
#define DATA_PIN    	13
#define LATCH_PIN   	12
#define CLOCK_PIN   	11
#define OE_PIN      	10
//
#define DELAY_US    	10

unsigned int secuences[22]={
   10832,
   10576,
   9808,
   9552,
   6736,
   6480,
   5712,
   5456,
   6728,
   6472,
   10800,
   10544,
   9776,
   9520,
   6704,
   6448,
   5680,
   5424,
   6696,
   6440,
   10800,
   5416
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
		os_strcpy(attenuation_string, buff);
		attenuation_db=atoi(attenuation_string);
		attenuatorController(attenuation_db);
		os_printf("Attenuator setted at %d dB\n", attenuation_db);
	}

	httpdRedirect(connData, "attenuatorController.tpl");
	return HTTPD_CGI_DONE;
}

//Template code for the attenuationController page.
int ICACHE_FLASH_ATTR tplAttenuationController(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return HTTPD_CGI_DONE;

	os_strcpy(buff, "Unknown");
	if (os_strcmp(token, "attenuation_string")==0) {
		os_strcpy(buff, attenuation_string);
	}
	httpdSend(connData, buff, -1);
	return HTTPD_CGI_DONE;
}

void attenuatorController(unsigned int attenuation_db){
	unsigned int status=0;
	unsigned int i=0;
	unsigned int secuence=secuences[attenuation_db];

	os_printf("\nAttenuator Controller:\n");
	for(i=0;i<CHANNELS;i++){
		status=secuence%2;
		secuence=secuence/2;

		if(status==0)
		{
			GPIO_OUTPUT_SET(DATA_PIN, LOW);
			os_printf("0");
		}
		else if(status==1)
		{
			GPIO_OUTPUT_SET(DATA_PIN, HIGH);
			os_printf("1");
		}
		GPIO_OUTPUT_SET(CLOCK_PIN, HIGH);
		os_delay_us(DELAY_US);
		GPIO_OUTPUT_SET(CLOCK_PIN, LOW);
		os_delay_us(DELAY_US);      
	}
	GPIO_OUTPUT_SET(LATCH_PIN, HIGH);
	os_delay_us(DELAY_US);
	GPIO_OUTPUT_SET(LATCH_PIN, LOW);
	os_delay_us(DELAY_US);
	os_printf("\n END OF PULSE TRAIN.\n");
}
