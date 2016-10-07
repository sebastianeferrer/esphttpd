#ifndef CGI_H
#define CGI_H

#include "httpd.h"

int cgiAttenuationController(HttpdConnData *connData);
int tplAttenuationController(HttpdConnData *connData, char *token, void **arg);
void attenuatorController(unsigned int attenuation_db);
#endif
