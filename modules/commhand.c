#include "commhand.h"

#define NOERROR 0

//global variables
int OVERFLOW = 0;

void start_commhand(){

  //sys_req(IDLE);

  int com = COM1;

  char args[50];
  char cmd[50];

  char *pargs = args;
  char *pcmd = cmd;

  int z=0;
  for (z = 0; z < 51; z++){
    pcmd[z] = '\0';
  }

  //need to clear the pargs
  for (z = 0; z < 51; z++){
    pargs[z] = '\0';
  }

  do {

    serial_print("$");
    //get input command
    get_cmd(com, pcmd);

    if (OVERFLOW == 0){
      //separate command from its arguements
      pargs = sep_args(pcmd);

      if (strcmp(pcmd, "exit") == 0 || strcmp(pcmd, "shutdown") == 0) {
        //clear all queues, then exit
        clearQs();
        sys_req(EXIT);
        //break;

      } else if (strcmp(pcmd, "version") == 0) {

        version(pargs);

      }
      // else if (strcmp(pcmd, "yield") == 0) { //TMP for R3
      //
      //   yield();
      //
      // }
      else if (strcmp(pcmd, "loadr3") == 0) {

        loadr3();

      } else if (strcmp(pcmd, "help") == 0) {

        help(pargs);

      } else if (strcmp(pcmd, "date") == 0) {

        date(pargs);

      } else if (strcmp(pcmd, "pcb") == 0){

        int result = processControl(pargs);
        if (result == -10){
          serial_println("Not a proper pcb command!");
        } else if (result == -2) {
          serial_println("Too many arguements given!");
        } else if (result == -3) {
          serial_println("PCB name has to be between 8 and 29 letters (most special characters are also invalid)");
        } else if (result == -4) {
          serial_println("Priority must be between 0 and 9");
        } else if (result == -5) {
          serial_println("Class must be 0 for System or 1 for Application");
        } else if (result == -6) {
          serial_println("Class and Priority must only be numbers between 0 and 9");
        } else if (result == -7) {
          serial_println("Not a valid name");
        } else if (result == -8) {
          serial_println("PCB doesn't exist");
        } else if (result == -9) {
          serial_println("PCB is already in that state");
        } else if (result == -11) {
          serial_println("PCB already exists!");
        }
      } else if (strcmp(pcmd, "showFree") == 0) {
        showFree();
      }
      // else if (strcmp(pcmd, "isEmpty") == 0) {
      //   int meow = isEmpty();
      //   if (meow == 1) {
      //     serial_println("The heap is empty bro");
      //   } else {
      //     serial_println("There's stuff in that heap");
      //   }
      // } else if (strcmp(pcmd, "mcb") == 0) {
      //  memoryManager(pargs);
      // }
      else if (strcmp(pcmd, "showAllocated") == 0) {
        showAllocated();
      }
      else {
        print_cmd("Not a valid command!");

      }

    } else {

      //error, just clear everything
      pcmd[50] = '\0';
      pargs[50] = '\0';
      OVERFLOW = 0;

    }

    for (z = 0; z < 50; z++){
      pcmd[z] = '\0';
    }

    //need to clear the pargs
    for (z = 0; z < 50; z++){
      pargs[z] = '\0';
    }

    sys_req(IDLE); //idle to let  processes run

  } while (1);

  //return NOERROR;
}

void get_cmd(int com, char *pcmd){
  int i = 0;
  while (1){
    if (inb(com+5)&1){
      if (i > 49){
        serial_println("You have entered too many characters");
        OVERFLOW = 1;
        break;
      }
      char in = inb(com);
      if (in == '\r' || in == '\n'){
        //pcmd[i++] = '\0';
        break;
      } else if (in == '\033'){
        if(inb(com) == '['){
          in = inb(com);
          if(in == 'D'){
            //Left arrow key
            if(i>0){
              outb(com, '\b');
              i--;
            }
          } else if(in == 'C'){
            //Right arrow key
            if(pcmd[i] != 0){
              outb(com, pcmd[i]);
              i++;
            }
          } else if(in == 'A'){
            //Up arrow key
          } else if(in == 'B'){
            //Down arrow key
          } else if(in == '3'){
            inb(com);
            //Delete key
            if(i>=0){
              if(pcmd[i] != NULL){
                int k = 0;
                while(pcmd[i] != NULL){
                  int j = i;
                  j++;
                  pcmd[i--] = pcmd[j];
                  outb(com, pcmd[j]);
                  i+=2;
                  k++;
                }
                outb(com, ' ');
                i--;
                pcmd[i] =NULL;
                while(!(k<1)){
                  outb(com, '\b');
                  i--;
                  k--;
                }
                i++;
              }
            }
          }
        }
      } else if (in == 127) {
        //handle backspace (backspace character is ascii value 127
        if(i>0){
          outb(com, in);
          if(pcmd[i] == NULL){
            outb(com, ' ');
            outb(com, '\b');
            //also clear it out of variable
            i--;
            pcmd[i] = NULL;
          } else{
            int k = 0;
            while(pcmd[i] != NULL){
              int j = i--;
              pcmd[i] = pcmd[j];
              outb(com, pcmd[i]);
              i+=2;
              k++;
            }
            outb(com, ' ');
            i--;
            pcmd[i] =NULL;
            while(!(k<0)){
              outb(com, '\b');
              i--;
              k--;
            }
            i++;
          }
        }
      } else {
        outb(com, in);
        pcmd[i++] = in;
      }
    }
  }

  /*//make sure to null terminate string
  while(*pcmd++ != NULL){
  i++;
}
pcmd[i++] = '\0';
*/
//print a new line to avoid overlap
outb(com, '\n');
}

char* sep_args(char *cmd){
  char cmd_args[50];
  char *pcmd_args = cmd_args;
  while (*cmd != '\0'){
    //checks for spaces in command line
    if (isspace(cmd)){
      //*cmd = '\0';
      cmd++;
      //separate command from the command arguements
      strcpy(pcmd_args, cmd);
      cmd--;
      //int i = 0;
      while(*cmd != '\0'){
        *cmd = '\0';
        cmd++;
      }
      break;
    }
    cmd++;
  }
  return pcmd_args;
}

void print_cmd(char *c){
  serial_println(c);
}
