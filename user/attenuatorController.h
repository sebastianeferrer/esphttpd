#ifndef ATTENUATORCONTROLLER_H
#define ATTENUATORCONTROLLER_H

#include "httpd.h"
#include "retval.h"

int cgiAttenuationController(HttpdConnData *connData);
int htmlAttenuationController(HttpdConnData *connData, char *token, void **arg);
retval_t attenuatorController(unsigned int attenuation_db);
void attenuatorInit(void);

#endif
