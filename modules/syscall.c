#include "syscall.h"

u32int* sys_call(context *registers){
  
  if (cop == NULL) {
    
    gcontext = registers;
    
  } else {
    
    if (params.op_code == IDLE) {

      cop->stacktop = (unsigned char *) registers;

      //reinsert into readyq
      RemovePCB(cop);
      InsertPCBReady(cop);
      
    } else if (params.op_code == EXIT) {
      
      DeletePCB(cop->name);
      
    }
  }

  cop = getNextProcess();
  
  if (cop == NULL){
    return (u32int *) gcontext;  
  }
  
  return (u32int *) cop->stacktop;
  
}

void yield(){
    asm volatile ("int $60");
}

void loadr3(){

  makeproc1();
  makeproc2();
  makeproc3();
  makeproc4();
  makeproc5();

}

void makeproc1(){
  CreatePCB("r3process1", "0", "0");
  pcb *new_pcb = FindPCB("r3process1");
  context *cp = (context *)(new_pcb->stacktop);
  memset(cp, 0, sizeof(context));
  cp->fs = 0x10;
  cp->gs = 0x10;
  cp->ds = 0x10;
  cp->es = 0x10;
  cp->cs = 0x8;
  cp->ebp = (u32int)(new_pcb->stack);
  cp->esp = (u32int)(new_pcb->stacktop);
  cp->eip = (u32int) proc1;
  cp->eflags = 0x202;
  suspendPCB("r3process1");
}

void makeproc2(){
  CreatePCB("r3process2", "1", "0");
  pcb *new_pcb = FindPCB("r3process2");
  context *cp = (context *)(new_pcb->stacktop);
  memset(cp, 0, sizeof(context));
  cp->fs = 0x10;
  cp->gs = 0x10;
  cp->ds = 0x10;
  cp->es = 0x10;
  cp->cs = 0x8;
  cp->ebp = (u32int)(new_pcb->stack);
  cp->esp = (u32int)(new_pcb->stacktop);
  cp->eip = (u32int) proc2;
  cp->eflags = 0x202;
  suspendPCB("r3process2");
}

void makeproc3(){
  CreatePCB("r3process3", "0", "0");
  pcb *new_pcb = FindPCB("r3process3");
  context *cp = (context *)(new_pcb->stacktop);
  memset(cp, 0, sizeof(context));
  cp->fs = 0x10;
  cp->gs = 0x10;
  cp->ds = 0x10;
  cp->es = 0x10;
  cp->cs = 0x8;
  cp->ebp = (u32int)(new_pcb->stack);
  cp->esp = (u32int)(new_pcb->stacktop);
  cp->eip = (u32int) proc3;
  cp->eflags = 0x202;
  suspendPCB("r3process3");
}

void makeproc4(){
  CreatePCB("r3process4", "1", "0");
  pcb *new_pcb = FindPCB("r3process4");
  context *cp = (context *)(new_pcb->stacktop);
  memset(cp, 0, sizeof(context));
  cp->fs = 0x10;
  cp->gs = 0x10;
  cp->ds = 0x10;
  cp->es = 0x10;
  cp->cs = 0x8;
  cp->ebp = (u32int)(new_pcb->stack);
  cp->esp = (u32int)(new_pcb->stacktop);
  cp->eip = (u32int) proc4;
  cp->eflags = 0x202;
  suspendPCB("r3process4");
}

void makeproc5(){
  CreatePCB("r3process5", "1", "0");
  pcb *new_pcb = FindPCB("r3process5");
  context *cp = (context *)(new_pcb->stacktop);
  memset(cp, 0, sizeof(context));
  cp->fs = 0x10;
  cp->gs = 0x10;
  cp->ds = 0x10;
  cp->es = 0x10;
  cp->cs = 0x8;
  cp->ebp = (u32int)(new_pcb->stack);
  cp->esp = (u32int)(new_pcb->stacktop);
  cp->eip = (u32int) proc5;
  cp->eflags = 0x202;
  suspendPCB("r3process5");
}


//get next pcb to run
pcb* getNextProcess(){
  
  pcb* runNext = readyq.head;
  while (runNext->suspendedstate == 1 && runNext != NULL){
    runNext = runNext->nextpcb;
  }

  return runNext;
  
}

