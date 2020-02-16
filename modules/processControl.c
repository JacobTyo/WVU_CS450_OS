#include "processControl.h"

int processControl(char *args){
  //figure out what args are and call appropriate function
  char flag[16], arg1[16], arg2[16], arg3[16], arg4[16];
  char *pflag = flag;//sys_alloc_mem(16);//flag;
  char *parg1 = arg1;//sys_alloc_mem(16);//arg1;
  char *parg2 = arg2;//sys_alloc_mem(16);//arg2;
  char *parg3 = arg3;//sys_alloc_mem(16);//arg3;
  char *parg4 = arg4;//sys_alloc_mem(16);//arg4;
  pflag = strtok(args, " "); //get first token (--whatever)
  parg1 = strtok(NULL, " "); //get second token (arg1)
  parg2 = strtok(NULL, " ");
  parg3 = strtok(NULL, " ");
  parg4 = strtok(NULL, " ");

  // if (strcmp(pflag, "--create") == 0){
  //
  //   //create pcb
  //   //does not check arg2 and 3 for numbers only
  //   if (parg4 == NULL || (strcmp(parg4, "") == 0)){
  //     int res = CreatePCB(parg1, parg2, parg3);
  //     return res;
  //   }
  //   return -2; //extra arguements given
  //
  // } else if (strcmp(pflag, "--delete") == 0){
  //
  //   if (parg2 == NULL || (strcmp(parg2, "") == 0)){
  //     int res = DeletePCB(parg1);
  //     return res;
  //   }
  //   return -2; //extra args given
  //
  // } else if (strcmp(pflag, "--block") == 0){
  //
  //   if (parg2 == NULL || (strcmp(parg2, "") == 0)){
  //     int res = BlockPCB(parg1);
  //     return res;
  //   }
  //   return -2; //extra args given
  //
  // } else if (strcmp(pflag, "--unblock") == 0){
  //
  //   if (parg2 == NULL || (strcmp(parg2, "") == 0)){
  //     int res = UnblockPCB(parg1);
  //     return res;
  //   }
  //   return -2; //extra args given

  // }
  if (strcmp(pflag, "--suspend") == 0){
    if (parg2 ==NULL || (strcmp(parg2, "") == 0)){
      int res = suspendPCB(parg1);
      return res;
    }
    return -2; //extra args given

  } else if (strcmp(pflag, "--resume") == 0){
    if (parg2 ==NULL || (strcmp(parg2, "") == 0)){
      int res = resumePCB(parg1);
      return res;
    }
    return -2; //extra args given

  }
  // else if (strcmp(pflag, "--show") == 0){
  //   if (parg2 ==NULL || (strcmp(parg2, "") == 0)){
  //     int res = ShowPCB(parg1);
  //     return res;
  //   }
  //   return -2; //extra args given
  // }
  else if (strcmp(pflag, "--show-all") == 0){
    if (parg1 ==NULL || (strcmp(parg1, "") == 0)){
      PrintReadyPCB();
      PrintBlockedPCB();
      return 0;
    }
    return -2; //extra args given
  } else if (strcmp(pflag, "--show-ready") == 0){
    if (parg1 ==NULL || (strcmp(parg1, "") == 0)){
      PrintReadyPCB();
      return 0;
    }
    return -2; //extra args given
  } else if(strcmp(pflag, "--show-blocked") == 0){
    if (parg1 ==NULL || (strcmp(parg1, "") == 0)){
      PrintBlockedPCB();
      return 0;
    }
    return -2; //extra args given
  } else if(strcmp(pflag, "--set-priority") == 0){
    if (parg3 == NULL || (strcmp(parg3, "") == 0)){
      return SetPriority(parg1, parg2);
    }
    return -2; //extra args given
  }else {

    return -10; //not a proper flag error

  }

  return 0;
}

int CreatePCB(char *name, char *class, char *priority){
  if (chkatoi(class) == 0 && chkatoi(priority) == 0){
    int iclass = atoi(class);
    int ipriority = atoi(priority);
    if (iclass == 0 || iclass == 1){
      if (ipriority >= 0 && ipriority <= 9){
        int namelen = strlen(name);
        if (namelen > 7 && namelen < 30 && (pcbNameCheck(name) == 0)){
          pcb *exists = FindPCB(name);
          if(exists == NULL){
            pcb *p = SetupPCB(name, iclass, ipriority);
            if (p != NULL){
              //creation was successful, insert into ready queue
              InsertPCBReady(p);
	      //for testing ShowPCB(p->name);
              return 0;
            }
          } else {
            return -11;
          }
        } else {
          return -3;
        }
      } else {
        return -4;
      }
    } else {
      return -5;
    }
  }
  return -6;
}

int DeletePCB(char *name){

  if (pcbNameCheck(name) == 0){
    //name is valid
    pcb *deleteme = FindPCB(name);
    int res = 0;
    int res2 = 0;
    if (deleteme != NULL){
      res = RemovePCB(deleteme);
      res2 = FreePCB(deleteme);
      int ret = res + res2;
      return ret;
    }else{
      return -8; // pcb doesn't exist
    }
  }
  return -7; //you have not entered a valid name
}

pcb* AllocatePCB(){

  pcb *pnewpcb = sys_alloc_mem(sizeof(pcb));
  return pnewpcb;

}

int FreePCB(pcb *fpcb){

  int error;
  error = sys_free_mem(fpcb);
  return error;

}

pcb* SetupPCB(char *name, int class, int priority){

  pcb *newpcb = AllocatePCB();

  if (strlen(name) < 30 && strlen(name) > 7){
    strcpy(newpcb->name, name);
  } else {
    return NULL;
  }

  newpcb->class = class;
  newpcb->priority = priority;
  newpcb->runningstate = 0;
  newpcb->suspendedstate = 0;
  newpcb->stacktop = newpcb->stack + 2048 - sizeof(context);

  return newpcb;

}

int pcbNameCheck(char *name){
  while (name != '\0' && !(strcmp(name, "") == 0)){
    if (isspace(name) == 1 || *name < 48 || (*name < 41 && *name > 58) || *name > 122 || *name == 92){
      return 1; //name is not valid
    }
    name++;
  }
  return 0; //name is valid
}


int BlockPCB(char *name){
  int ret = 0;
  pcb *blockme = NULL;

  //check to see if name is valid
  if (pcbNameCheck(name) == 0){
    blockme = FindPCB(name);

    //check to see if pcb name exists
    if(blockme != NULL){

      //check to see if pcb is already blocked
      if(blockme->runningstate != 2){
	//need to remove only, because deleting actually frees the space it occupys
        RemovePCB(blockme);
        blockme->runningstate = 2;
        InsertPCBBlocked(blockme);
      } else {
        //pcb is already in blocked state
        ret = -9;
      }
    } else {
      //pcb does not exist
      ret = -8;
    }
  } else {
    //pcb name is not valid
    ret = -7;
  }
  return ret;
}

int UnblockPCB(char *name){
  int ret = 0;
  pcb *unblockme;

  //check to see if name is valid
  if (pcbNameCheck(name) == 0){
    unblockme = FindPCB(name);
    //check to see if pcb name exists
    if(unblockme != NULL){
      //check to see if pcb is already ready
      if(unblockme->runningstate != 0){
	//need to use RemovePCB instead of Delete because Delete takes removes the allocated memory that the pcb uses
        RemovePCB(unblockme);
        unblockme->runningstate = 0;
        InsertPCBReady(unblockme);
      } else {
        //pcb is already in ready state
        ret = -9;
      }
    } else {
      //pcb does not exist
      ret = -8;
    }
  } else {
    //pcb name is not valid
    ret = -7;
  }
  return ret;
}

int suspendPCB(char *name){
  int ret = 0;
  pcb *suspendme;

  if (pcbNameCheck(name) == 0){ //PCB has valid name
    if (FindPCB(name) != NULL){

      suspendme = FindPCB(name);
      //check to see if already suspended
      if (suspendme->suspendedstate != 1){
        //suspend PCB
        suspendme->suspendedstate = 1;
      }
      else{
        ret = -9; //already suspended
      }
    }
    else {
      //pcb does not exist
      ret = -8;
    }
  }
  else {
    ret = -7; //PCB name not valid
  }

  return ret;
}

int resumePCB(char *name){
  int ret = 0;
  pcb * resumeme;

  if (pcbNameCheck(name) == 0){ //PCB has valid name
    if (FindPCB(name) != NULL){
      resumeme = FindPCB(name);
      //check to see if already resumed
      if (resumeme -> suspendedstate != 0){
        //resume PCB
        resumeme -> suspendedstate = 0;
      }
      else{
        ret = -9; //already suspended
      }
    }
    else {
      //pcb does not exist
      ret = -8;
    }
  }
  else {
    ret = -7; //PCB name not valid
  }

  return ret;
}


int ShowPCB(char *name){
  //int ret = 0;
  pcb *showme;
  if (pcbNameCheck(name) == 0){
    showme = FindPCB(name);
    if(showme != NULL){
      PrintPCB(showme);
      return 0;
    } else{
      return -8;
    }
  } else {
    return -7; //PCB name not valid
  }
}

int SetPriority(char *name, char *ppriority){
  pcb *pcb;
  //check to see if name is valid
  if (pcbNameCheck(name) == 0){
    pcb = FindPCB(name);
    //check to see if pcb name exists
    if(pcb != NULL){
      //check to see if pcb is already ready
      if(chkatoi(ppriority) == 0){
        int priority = atoi(ppriority);
        if (pcb->priority != priority) {
	  if (priority >= 0 && priority < 10){
	    RemovePCB(pcb);
	    pcb->priority = priority;
	    //reinserts it into the correct queue
	    if(pcb->runningstate == 0 || pcb->runningstate == 1){
	      InsertPCBReady(pcb);
	    } else if(pcb->runningstate == 2){
	      InsertPCBBlocked(pcb);
	    }
	    return 0;
	  } else {
	    //priority is not between 0 and 9
	    return -6;
	  }

	} else {
	  //pcb is already this priority
	  return -9;
	}
      } else {
	return -4;
      }
    } else {
      //pcb does not exist
      return -8;
    }
  } else {
    //pcb name is not valid
    return -7;
  }
}
