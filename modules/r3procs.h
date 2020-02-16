/** @file */
#ifndef _R3PROCS_H
#define _R3PROCS_H

#include <system.h>
#include <core/serial.h>

#include "mpx_supt.h"

#define RC_1 1
#define RC_2 2
#define RC_3 3
#define RC_4 4
#define RC_5 5

/*
  This function was provided to the developers to be implemented as a process. Specifically, this process will run once, then exit. 
 */
void proc1();

/*
  This function was provided to the developers to be implemented as a process. Specifically, this process will run once, then IDLE (give up the cpu), then EXIT after the next time it is ran.  
 */
void proc2();

/*
  This function was provided to the developers to be implemented as a process.  Specifically, this process will run three times, IDLEing twice, then EXITing on the third iteration. 
 */
void proc3();

/*
  This function was provided to the developers to be implemented as a process.  Specifically, this process will run four times, ILDEing three times, then EXITing on the fourth iteration. 
 */
void proc4();

/*
  This function was provided to the developers to be implemented as a process.  Specifically, this process will run five times, ILDEing four times, then EXITint on the fifth iteration. 

 */
void proc5();

#endif
