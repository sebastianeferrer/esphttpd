#include <esp8266.h>
#include "log.h"
#include "retval.h"

int ICACHE_FLASH_ATTR cgiLog(HttpdConnData *connData) {
	return HTTPD_CGI_DONE;
}

int ICACHE_FLASH_ATTR htmlLog(HttpdConnData *connData, char *token, void **arg) {
	return HTTPD_CGI_DONE;
}
