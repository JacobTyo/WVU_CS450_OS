/** @file */
#ifndef _PROCESSCONTROL_H
#define _PROCESSCONTROL_H

#include <string.h>
#include "mpx_supt.h"
#include "extFuncs.h"
#include "queues.h"

/**
   This pcb struct typedef is in place in order to allow for placing a pointer to the pcb structure, inside of the struct definition of the pcb.
 */
typedef struct pcb pcb;

/**
  This context struct typedef is in place in order to allow for creating a context without the use of struct.
 */
typedef struct context context;

/**
   The pcb structure is a Process Control Block.  This block consists of the follwing variables:
   char name[11] - the name of the Process Control Block
   int class - the process class (0 = system, 1 = application)
   int priority - integer between 0 and 9 representing the execution priority with 0 being the highest priority and 9 being the lowest priority.
   int runningstate - the processes ready(0), running(1), or blocked(2) state.
   int suspendedstate - the process suspended(1), or not suspended(0) state.
   char stack[4098] - the stack area of 1025 bytes.
   unsigned char *stacktop - the pointer to the top of the stack.
   unsigned char *stackbase - the pointer to the base of the stack.
   pcb *nextpcb - a pointer to the pcb next in line for execution.
   pcb *prevpcb - a pointer to the pcb previous in line for execution.
 */
struct pcb {
  char name[31];
  int class, priority, runningstate, suspendedstate;
  unsigned char stack[2048];
  unsigned char *stacktop;
  unsigned char *stackbase;
  pcb *nextpcb;
  pcb *prevpcb;
};

/**
   This context structure is used for context switching.  The variables contained within this struct are:
   gs, fs, es, ds - data segment registers
   edi esi, ebp, esp, ebx, edx, ecx, eax - 32-bit general-purpose registers
   eip - instruction pointer
   cs - code segment
   eflags - a 32-bit flags register containing groups of flags
   
 */
struct context {
  u32int gs, fs, es, ds;
  u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
  u32int eip, cs, eflags;
};

/**
   The processControl function acts as the main interface between the commhand and itself.  This function takes the arguements passed to it as the input, and returns an integer indicating success or error code.
 */
int processControl(char *args);

/**
   The CreatePCB Function takes a character pointer to the name, class, and priority of the pcb to be created as inputs, and returns an integer indicating success or error.  This function checks for valid input and then create a new pcb with the input information and add it to the ready queue, in the ready, not suspended state.
   returned error integers:
     -1 -> input class or priority contians more than just numbers
     -2 -> class is a number other than 1 or 0
     -3 -> priority is not between 0 and 9
     -4 -> name is not of proper length or is not unique
 */
int CreatePCB(char *name, char *class, char *priority);

/**
   The DeletePCB function takes the name of a pcb in as input, then deletes the coresponding pcb and frees the associated memory.
 */
int DeletePCB(char *name);

/**
   The AllocatePCB function is used to allocate memory for a new PCB. No argument is taken, but a pointer to the newly allocated memory is returned.
 */
pcb* AllocatePCB();

/**
   The FreePCB function is used to free memory that used to contain a PCB. A pointer to a PCB is passed into the function, and the memory is then freed. An integer is returned indicating success (0) or failure (else).
 */
int FreePCB(pcb *fpcb);

/**
   The SetupPCB funciton is used to create and initialize a new PCB with a name, class, priority, runningstate to ready, and suspendedstate to notsuspended.  The function inputs and return follow:
   char *name - desired name of the new PCB (between 8 and 30 characters)
   int class - the desired process class
   int priority - the desired process priority
   return pcb* - the pointer to the newly created and initialized PCB.
 */
pcb* SetupPCB(char *name, int class, int priority);

/**
   The pcbNameCheck function takes a character pointer to a character array (string) and then checks to see if it is a valid name for a PCB.
 */
int pcbNameCheck(char *name);

/**
   The BlockPCB function takes a character pointer to a character array (string) and then finds the PCB. If the PCB exists the function then places the PCB in the blocked state and reinserts it into the appropriate queue
*/
int BlockPCB(char *name);

/**
   The Unblock function takes a character pointer to a character array (string) and then finds the PCB. If the PCB exists the function places the PCB in the unblocked state and reinserts it into the appropriate queue
*/
int UnblockPCB(char *name);

/**
   The suspendPCB function takes a character pointer to a character array (string) and then finds the PCB. If the PCB exists the function changes the suspended state of the PCB to suspended
*/
int suspendPCB(char *name);

/**
   The resumePCB function takes a character pointer to a character array (string) and then finds the PCB. If the PCB exists the function changes the suspended state of the PCB to not suspended
*/
int resumePCB(char *name);

/**
  The ShowPCB function takes a character pointer to a character array (string) and then finds the PCB. If the PCB exists the function then calls on PrintPCB
*/
int ShowPCB(char *name);

/**
  The SetPriority function takes two character pointers. One to a character array (string) and then finds the PCB and the other other is converted to an int
  adn then set the to priority.
*/
int SetPriority(char *name, char *priority);

#endif
