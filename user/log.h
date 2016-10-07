#ifndef LOG_H
#define LOG_H

#include "httpd.h"
#include "retval.h"

int cgiLog(HttpdConnData *connData);
int htmlLog(HttpdConnData *connData, char *token, void **arg);

#endif
