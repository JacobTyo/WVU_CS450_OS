#include "queues.h"

queue readyq;
queue blockedq;

void init_queues(){
  readyq.count = 0;
  readyq.head = NULL;
  readyq.tail = NULL;

  blockedq.count = 0;
  blockedq.head = NULL;
  blockedq.tail = NULL;
}

void InsertPCBReady(pcb *p){
  
  if (readyq.count == 0){
    
    //this is first block being added to the q
    readyq.head = p;
    p->nextpcb = NULL;
    p->prevpcb = NULL;
    readyq.count = 1;
    
  } else {
    //insert pcb in proper location
    pcb *tmp;
    tmp = readyq.head;
    //insert as head if it is the new, lowest priority pcb
    if (p->priority < tmp->priority){
      readyq.head = p;
      p->nextpcb = tmp;
      tmp->prevpcb = p;
      p->prevpcb = NULL;
    } else {
      //find last pcb of equal priority to p
      while (tmp->priority <= p->priority){
	if (tmp->nextpcb == NULL || tmp->nextpcb->priority > p->priority){
	  break;
	}
	tmp = tmp->nextpcb;
      }
      //insert pcb after tmp
      p->nextpcb = tmp->nextpcb;
      tmp->nextpcb = p;
      if (p->nextpcb != NULL){
	p->nextpcb->prevpcb = p;
      }
      p->prevpcb = tmp;
    }
    readyq.count = readyq.count + 1;
  }
}

void InsertPCBBlocked(pcb *p){
  if (blockedq.count == 0){
    //pcb should be at the head of the queue, pointing to nothing because it is the only thing there
    blockedq.head = p;
    p->nextpcb = NULL;
    p->prevpcb = NULL;
    blockedq.count = 1;

  } else {
    pcb *tmp = blockedq.head;
    while (tmp->nextpcb != NULL){
      tmp = tmp->nextpcb;
    }
    tmp->nextpcb = p;
    p->prevpcb = tmp;
    p->nextpcb = NULL;
    blockedq.count = blockedq.count + 1;

  }
}

int RemovePCB(pcb *p){
  if (p != NULL){
    pcb *previouspcb;
    pcb *followingpcb;
    previouspcb = p->prevpcb;
    followingpcb = p->nextpcb;
    previouspcb->nextpcb = followingpcb;
    followingpcb->prevpcb = previouspcb;

    if (p == readyq.head){
      readyq.head = readyq.head->nextpcb;
    } else if (p == blockedq.head){
      blockedq.head = blockedq.head->nextpcb;
    }
    
    //update count for queue that p was removed from - can use runningstate to determine which q it came from
    if (p->runningstate == 0 || p->runningstate == 1){
      //remove from readyq
      readyq.count = readyq.count - 1;
    } else {
      blockedq.count = blockedq.count - 1;
    }

    return 0;
  } else {
    return -1;
  }
}

pcb* FindPCB(char *name){
  //look through ready and then blocked queues for pcb
  pcb *found = NULL;
  pcb *current = readyq.head;
  while (current != NULL){
    if (strcmp(current->name, name) == 0){
      //pcb found
      found = current;
      break;
    }
    current = current->nextpcb;
  }
  //moved to blocked q
  current = blockedq.head;
  while (current != NULL && found == NULL){
    if (strcmp(current->name, name) == 0){
      found = current;
      break;
    }
    current = current->nextpcb;
  }
  return found;
}

void PrintPCB(pcb *pcb){
  serial_print("Name: ");
  serial_println(pcb->name);
  if(pcb->class == 0) {
    serial_println("\tClass: System");
  } else if(pcb->class == 1){
    serial_println("\tClass: Application");
  }
  if(pcb->runningstate == 0){
    serial_println("\tState: Ready");
  } else if(pcb->runningstate == 1){
    serial_println("\tState: Running");
  } else if (pcb->runningstate ==2){
    serial_println("\tState: Blocked");
  }
  if(pcb->suspendedstate == 0){
    serial_println("\tStatus: Not suspended");
  } else if (pcb->suspendedstate == 1){
    serial_println("\tStatus: Suspended");
  }
  serial_print("\tPriority: ");
  print_int(pcb->priority);
  serial_println("");
  serial_println("");
}

void PrintReadyPCB(){
  pcb *pcb;
  pcb = readyq.head;
  while (pcb != NULL){
    PrintPCB(pcb);
    pcb = pcb->nextpcb;
  }
}

void PrintBlockedPCB(){
  pcb *pcb;
  pcb = blockedq.head;
  while (pcb != NULL){
    PrintPCB(pcb);
    pcb = pcb->nextpcb;
  }
}

void clearQs(){

  //clear blockedq
  while (blockedq.count != 0){
    DeletePCB(blockedq.head->name);
  }

  //clear readyq
  while (readyq.count != 1){
    if (strcmp(readyq.head->name, "commhand") == 0){
      DeletePCB(readyq.head->nextpcb->name);
    } else {
      DeletePCB(readyq.head->name);
    }
  }
  
}
