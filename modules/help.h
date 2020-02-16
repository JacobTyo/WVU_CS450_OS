/** @file */
#ifndef _HELP_H
#define _HELP_H

#include <string.h>
#include <system.h>
#include <core/serial.h>
#include <core/io.h>

/*
  The help function is the main interface to the operating systems help.  It takes a char pointer to the arguements, and then passes them to the necessary command.
 */
void help(char *args);

/*
  This is the function that handles the help dialog for version.  The input and return values are void.
 */
void version_help();

/*
  The shutdown_help function displays the help dialog for the shutdown command.  The input and return values are void.
 */
void shutdown_help();

/*
  The date_help function displays the help dialog for the date command. There are no inputs or returned values.
 */
void date_help();

/*
  The pcb_help function displays the help dialog for the pcb command.  There are no inputs or returned values.
 */
void pcb_help();

/*
  The yield_help function displays the help dialog for the yield command.  There are no inputs or returned values.
 */
void yield_help();

/*
  The loadr3_help function displays the help dialog for the loadr3 command.  There are no inputs or returned values.
 */
void loadr3_help();

/*
  The showAllocated_help function displays the help dialog for the showAllocated_help command.  There are no inputs or returned values.
 */
void showAllocated_help();

/*
  The showFree_help function displays the help dialog for the showFree_help command.  There are no inputs or returned values.
 */
void showFree_help();

/*
  The default_help method displays the help dialog when no arguement, or and invalid arguement is given.
 */
void default_help();

#endif
