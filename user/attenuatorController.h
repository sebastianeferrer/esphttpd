#ifndef CGI_H
#define CGI_H

#include "httpd.h"

/* commonly used for I/O operations */
typedef enum {
    RV_SUCCESS = 0,
    RV_TIMEOUT = 1,             /* timer expired */
    RV_BUSY = 2,                /* queue full, bus locked for contention, etc... */
    RV_ILLEGAL = 3,             /* illegal/invalid call arguments */
    RV_NOSPACE = 4,             /* space not available */
    RV_ERROR = 5,               /* any other error */
    RV_EXIST = 6,               /* resource exists already */
    RV_NOENT = 7,               /* entry not available */
    RV_PERM = 8,                /* call not permitted */
    RV_PARTIAL = 9,             /* partial operation successfully accomplished */
    RV_NACK = 10,               /* everything was alright, except the answer was negative */
    RV_NOTIMPLEMENTED = 11
} retval_t;

int cgiAttenuationController(HttpdConnData *connData);
int tplAttenuationController(HttpdConnData *connData, char *token, void **arg);
retval_t attenuatorController(unsigned int attenuation_db);

#endif
