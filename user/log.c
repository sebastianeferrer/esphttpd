#include <esp8266.h>
#include "log.h"
#include "retval.h"

int ICACHE_FLASH_ATTR cgiLog(HttpdConnData *connData) {
	int len;
	char buff[1024];
	
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	httpdRedirect(connData, "log.html");
	return HTTPD_CGI_DONE;
}

int ICACHE_FLASH_ATTR htmlLog(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return HTTPD_CGI_DONE;

	httpdSend(connData, buff, -1);
	return HTTPD_CGI_DONE;
}
