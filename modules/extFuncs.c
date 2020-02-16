#include "extFuncs.h"

void print_int(int printme){
  
  if (printme > 9 || printme < 0){
    int newprintme = printme/10;
    print_int(newprintme);
  }
  
  int print = printme % 10;
  outb(COM1, print+'0');
  
}

int chkatoi(char *number){
  while (*number != '\0'){
    if (*number < '0' || *number > '9'){
      return 1; //does not contain only numbers
    }
    number++;
  }
  return 0; //contains only numbers
}
