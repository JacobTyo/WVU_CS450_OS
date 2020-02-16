/** @file */
#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "processControl.h"
#include "queues.h"
#include "r3procs.h"
#include "mpx_supt.h"

/*
  In order to get the parameter code of a process (i.e. IDLE or EXIT), we must get the external variable which is done here with params.
 */
extern param params;

/*
  cop stands for the currently operating process.  This pcb pointer will keep track of the process that currently has use of the processor. 
 */
pcb *cop;

/*
  In order to return to normal operation after all processes are completed (or when exiting the system), a global context pointer was used to store, and later return the original registers and associated values.
 */
context *gcontext;

/*
  The sys_call function is called when an interrupt is deteced on line 60, and then determins which process gets use of the cpu next. This function takes a context pointer as input which signifies the registers values currently being used, and returns a u32int pointer which contains the location of the stack of the process to be ran next.  
 */
u32int* sys_call(context *registers);

/*
  The yield process generates an interrupt on line 60, which allows a process to give up control of the cpu. 
 */
void yield();

/*
  The loadr3 function is used to load all of the functions given for r3 into the system as processes. This will load all functions with priority 0 and in the ready suspended state.  
 */
void loadr3();

/*
  This process is called by loadr3, and simply loads process 1 into the system. 
 */
void makeproc1();

/*
  This process is called by loadr3, and simply loads process 2 into the system. 
 */
void makeproc2();

/*
  This process is called by loadr3, and simply loads process 3 into the system. 
 */
void makeproc3();

/*
  This process is called by loadr3, and simply loads process 4 into the system. 
 */
void makeproc4();

/*
  This process is called by loadr3, and simply loads process 5 into the system. 
 */
void makeproc5();

/*
  The getNextProcess function selects the next non-suspended process from the ready queue and returns a pointer to said process.  
 */
pcb* getNextProcess();

#endif
