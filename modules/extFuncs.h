/** @file */
#ifndef _EXTFUNCS_H
#define _EXTFUNCS_H

#include <core/serial.h>
#include <core/io.h>
#include <string.h>

/**
   The print_int function will print an integer of any length to the screen. 
 */
void print_int(int printme);

/**
   The chkatoi function will make sure than a character array being passed to the atoi function contains only integers.  Otherwise it will thrown an error. 
 */
int chkatoi(char *number);

#endif
