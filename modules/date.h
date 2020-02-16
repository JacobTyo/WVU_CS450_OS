/** @file */
#ifndef _TIME_H
#define _TIME_H

#include <string.h>
#include <system.h>
#include <core/serial.h>
#include <core/io.h>
#include "mpx_supt.h"

/*
  The date function stars the date interface.  With no arguements, it just displays the current date and time.  If the --set flag is given, the date and time will be set. This takes a character pointer to the arguements as input and has no return value.  
 */
void date(char * args);

/*
  The hex2ASCII will convert a hex value to ascii.  This function takes a unsigned char as input and returns an unsigned char as the newly converted ascii value.
 */
unsigned char hex2ASCII(unsigned char dec);

/*
  The ASCII2hex function will convert an ascii value to hex. This function takes an unsigned char as input, and retuns the unsigned char of the newly converted hex value.  
 */
unsigned char ASCII2hex(char *ASCII);

#endif
