/** @file */
#ifndef _QUEUES_H
#define _QUEUES_H

#include <string.h>
#include "processControl.h"
#include "extFuncs.h"

/**
   The typedef struct queue allows for the use of queue as a data type without having to type struct queue every time. 
 */
typedef struct queue queue;

/** 
    The typedef struct pcb allows for the use of pcb as a data type without having to type struct every time. 
 */
typedef struct pcb pcb;

/**
   The queue structure is used to house the number of elements, and the first and last node in the queue.  This structure consists of an int for the number of nodes, and a pcb pointer to the head and tail of the queue.  
 */
struct queue{
  int count;
  pcb *head;
  pcb *tail;
};

/**
   The readyq is the queue used by the operating system to keep track of the ready processes. This is defined here to make it global.
 */
extern queue readyq;

/**
   The blockedq is the queue used by the operating system to keep track of the blocked processes.  This is defined here to make it global. 
 */
extern queue blockedq;

/**
   The init_queues function initializes a ready and a blocked queue setting the count to zero, and the head and tail to NULL.
 */
void init_queues();

/**
   The InsertPCBReady function will insert the givin pcb into the ready queue in the appropriate location.  The ready queue is a priority queue.
 */
void InsertPCBReady(pcb *p);

/**
   The InsertPCBBlocked functino will insert the givin pcb at the end of the blocked queue.  The blocked queue is a first in first out queue.
 */
void InsertPCBBlocked(pcb *p);

/**
   The RemovePCB function will remove the pcb from whichever queue it is currently housed in.
 */
int RemovePCB(pcb *p);

/**
   The FindPCB function takes in the name of a function and searches all queues for it.  If an exact match to the function name is found, it will return the pointer to the pcb, otherwise it will return NULL.
 */
pcb* FindPCB(char *name);

/**
  The PrintPCB function takes in a pcb and prints it out
*/
void PrintPCB(pcb *pcb);

/**
  The PrintReadyPCB function traverses the ready queue and calls PrintPCB for each pcb
*/
void PrintReadyPCB();

/**
  The PrintBlockedPCB function traverses the blocked queue and calls PrintPCB for each pcb
*/
void PrintBlockedPCB();

/**
   Clear all queues except for the commhand process.
 */
void clearQs();
#endif
