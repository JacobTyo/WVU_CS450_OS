#include "help.h"

void help(char *args){
  if (strlen(args) == 0){
    default_help();
  } else if (strcmp(args, "version") == 0){
    //display version help
    version_help();
  } else if (strcmp(args, "shutdown") == 0){
    //display shutdown/exit help
    shutdown_help();
  } else if (strcmp(args, "exit") == 0){
    //display shutdown/exit help
    shutdown_help();
  } else if (strcmp(args, "date") == 0){
    //display date help
    date_help();
  } else if (strcmp(args, "pcb") == 0){
    pcb_help();
  }
  // else if (strcmp(args, "yield") == 0){
  //   yield_help();
  // }
  else if (strcmp(args, "showAllocated") == 0) {
    showAllocated_help();
  } else if (strcmp(args, "showFree") == 0) {
    showFree_help();
  } else {
    serial_println("This isn't a valid command!");
    default_help();
  }
}

void version_help(){
  serial_println("\nUsage: \n\tversion");
  serial_println("Description: \n\tThe version command displays the current version of the MPX operating system");
  serial_println("Arguements: \n\tnone");
}

void shutdown_help(){
  serial_println("\nUsage:\n\tshutdown");
  serial_println("Description:\n\tThe shutdown command will halt all processes and shut the operating system down.");
}

void date_help(){
  serial_println("\nUsage:\n\tdate [ARGUEMENT] [MM-DD-YR HH:MM:SS]");
  serial_println("Description:\n\tthe date command can be used to set or check the current system time");
  serial_println("Arguements:\n\t--set MM-DD-YY HH:MM:SS");
}

void pcb_help(){
  serial_println("\nUsage:\n\tpcb");
  serial_println("Description:\n\tThe pcb command is used for manipulating the pcb");
  serial_println("Arguements:\n\t--create NAME CLASS PRIORITY\n\t--delete NAME\n\t--block NAME\n\t--unblock NAME\n\t--suspend NAME\n\t--resume NAME");
  serial_println("\t--show NAME\n\t--show-all\n\t--show-ready\n\t--show-blocked\n\t--set-priority NAME PRIORITY");
}

void yield_help(){
  serial_println("\nUsage:\n\tyield");
  serial_println("Description:\n\tThe yield command is used to manually cause commhand to yield control to other processes");
  serial_println("Arguements:\n\t none");
}

void loadr3_help(){
  serial_println("\nUsage:\n\tloadr3");
  serial_println("Description:\n\tThe loadr3 command is used to load all r3 processes into memory");
  serial_println("Arguements:\n\t none");
}

void showAllocated_help(){
  serial_println("\nUsage:\n\tshowAllocated");
  serial_println("Description:\n\tShows allocated memory");
  serial_println("Arguements:\n\t none");
}

void showFree_help(){
  serial_println("\nUsage:\n\tshowFree");
  serial_println("Description:\n\tShows free memory");
  serial_println("Arguements:\n\t none");
}


void default_help(){
  serial_println("\nCommands available are: \n\thelp \n\tversion \n\tdate \n\tshutdown \n\texit \n\tpcb \n\tloadr3 \n\tshowAllocated \n\tshowFree");
}
