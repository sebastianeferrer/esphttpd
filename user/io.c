
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */


#include <esp8266.h>

#define WIFI_VERBOSE
#define LEDGPIO 			0
#define BTNGPIO 			2
#define WIFI_TASK_TIME		500
#define WIFI_WDT_SEG 	 	240
#define WIFI_WDT_MSEG 		WIFI_WDT_SEG*(1000/WIFI_TASK_TIME)
/*
user_interface.h:
#define NULL_MODE       0x00
#define STATION_MODE    0x01
#define SOFTAP_MODE     0x02
#define STATIONAP_MODE  0x03
*/
static ETSTimer resetWIFItimer;

void ICACHE_FLASH_ATTR ioLed(int ena) {
	//gpio_output_set is overkill. ToDo: use better mactos
	if (ena) {
		gpio_output_set((1<<LEDGPIO), 0, (1<<LEDGPIO), 0);
	} else {
		gpio_output_set(0, (1<<LEDGPIO), (1<<LEDGPIO), 0);
	}
}

static void ICACHE_FLASH_ATTR resetWIFItimerCb(void *arg) {
	static int button_count=0;
	static int wifi_count=0;
	struct ip_info pTempIp;
	bool rv=-1;
	char buf[64]="000.0000.000.000EXTRA";
	uint32 heartbeat=0;
	const char* const wifi_modes[]={
		"STATION_IDLE",
		"STATION_CONNECTING",
		"STATION_WRONG_PASSWORD",
		"STATION_NO_AP_FOUND",
		"STATION_CONNECT_FAIL",
		"STATION_GOT_IP"
	};
#ifdef WIFI_VERBOSE
	os_printf("START--------------WIFI_VERBOSE--------------\n");
	heartbeat=system_get_rtc_time()*system_rtc_clock_cali_proc();
	os_printf("HEARTBEAT: %u\n", heartbeat);
	os_printf("GPIO_INPUT: %d\n",GPIO_INPUT_GET(BTNGPIO));
	os_printf("WIFI_STATUS: %s WIFI_WDT:%d\n\n",wifi_modes[wifi_station_get_connect_status()],wifi_count);
   	rv=wifi_get_ip_info(0x00, &pTempIp);
	if(!rv)os_printf("FAILED GETTING STATION IP\n");
	else {
		os_sprintf(buf, IPSTR,IP2STR(&pTempIp.ip));
		os_printf("STATION IP: %s\n",buf);
	}
   	rv=wifi_get_ip_info(0x01, &pTempIp);
	if(!rv)os_printf("FAILED GETTING AP IP\n");
	else {
		os_sprintf(buf, IPSTR,IP2STR(&pTempIp.ip));
		os_printf("AP IP: %s\n",buf);
	}
	os_printf("SDK version: %s\n", system_get_sdk_version());
	os_printf("--------------WIFI_VERBOSE--------------END\n");
#endif   	
	switch(wifi_get_opmode()){
		case STATION_MODE:
		case STATIONAP_MODE:
			if(STATION_GOT_IP!=wifi_station_get_connect_status()) wifi_count++;
			break;
		case SOFTAP_MODE:
			os_timer_disarm(&resetWIFItimer);
			break;
		case NULL_MODE: 
		default:
			break;
	}
	if (!GPIO_INPUT_GET(BTNGPIO)) button_count++;
	if (button_count>=6 || wifi_count>WIFI_WDT_MSEG) {
		wifi_station_disconnect();
		wifi_set_opmode(SOFTAP_MODE); //reset to SOFTAP_MODE mode
		os_printf("Reset to AP mode. Restarting system...\n");
		system_restart();
		button_count=0;
		wifi_count=0;
	}
}

void ioInit() {
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
	gpio_output_set(0, 0, (1<<LEDGPIO), (1<<BTNGPIO));
	os_timer_disarm(&resetWIFItimer);
	os_timer_setfn(&resetWIFItimer, resetWIFItimerCb, NULL);
	os_timer_arm(&resetWIFItimer, WIFI_TASK_TIME, 1);
}

