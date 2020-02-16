#include "version.h"

void version(char *args){
  char *c = "MPX R5 (04/8/2016)";
  if (*args == NULL || (strcmp(args, "") == 0)){
    serial_println(c);
  } else {
    serial_println("You have entered invalid arguements.  Try 'help version' for the propper useage of this command");
  }
}
