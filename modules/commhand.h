/** @file */
#ifndef _COMMHAND_H
#define _COMMHAND_H

#include <system.h>
#include <string.h>
#include <core/serial.h>
#include <core/io.h>

#include "version.h"
#include "help.h"
#include "date.h"
#include "processControl.h"
#include "extFuncs.h"
#include "queues.h"
#include "syscall.h"
#include "mpx_supt.h"
#include "mcb.h"

/*!
  Start the interface for interacting with the user and the system.  This function takes in the integer that allows for communication through outb, and returns an int which represents any error that may have occured.
 */
void start_commhand();

/*!
  The get_cmd function will get a line from the command line to be processed by the system.  The input is the in of the system com device, and a char pointer to the array to store the input command in.
 */
void get_cmd(int com, char *pcmd);

/*!
  The sep_args function takes a character pointer as input, then separates the first word from the rest.  This theoretically (and pragmatically) separates the command from its arguements for later processing in the system.  The input is the character pointer to the string to be processed
 */
char* sep_args(char *cmd);

/*!
  The print_cmd command will simply print the current command to the screen.  This was implemented for testing and is now dilapidated.
 */
void print_cmd();

#endif
