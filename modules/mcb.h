/** @file */
#ifndef _MCB_H
#define _MCB_H

#include "extFuncs.h"
#include "string.h"
#include "system.h"
#include "mem/heap.h"


/**
   This cmcb struct typedef is in place in order to allow for placing a pointer to the cmcb structure, inside of the struct definition of the cmcb.
 */
typedef struct cmcb cmcb;

/**
   This lmcb struct typedef is in place in order to allow for placing a pointer to the lcmb structure, inside of the struct definition of the cmcb
*/
typedef struct lmcb lmcb;

/**
   This list struct typedef is in place in order to allow for placing a pointer to the list structure, inside of the struct definition of the cmcb
*/
typedef struct list list;

/**
   The cmcb structure is a Complete Memory Control Block. This block consists of the following variable:
   int allocated - state of block, is allocated (1), and not allocated (0).
   int size - the size of the memory control block
   char name[31] - the name of the complete memory control block
   u32int beginningAddress - the beginning addresss of the memory control block
   lmcb *thisLmcb - a pointer to the lcmb of the corresponding cmcb
   cmcb *nextcmcb - a pointer to the next cmcb of the same state
   cmcb *prevcmcb - a pointer to the previous cmcb of the same state
*/
struct cmcb {
  int allocated;
  char name[31];
  u32int beginningAddress, size;
  lmcb *thisLmcb;
  cmcb *nextcmcb;
  cmcb *prevcmcb;
};

/**
   This lmcb structure is a Limtited Memory Control Block. This block corresponds to a specific Complete Memory Control Block, and contains the same size and allocated state. This block consists of the following variables:
   int allocated - state of the block, allocated (1), and not allocated (0)
   int size - the size of the Memory Control Block
*/
struct lmcb {
  int allocated;
  u32int size;
};

/**
   The list struct is the structure of the list used for the free and allocated lists. This list consists of the following variables:
   cmcb *head - a pointer to the head cmcb.
*/
struct list {
  cmcb *head;
};

/**
   Gloabal variables for the free list and allocated list.
*/
extern list freeList;
extern list allocatedList;

/**
   The memoryManager function acts as the main interface between the commhand and itself. This function takes the arguements passed to it as the input, and returns an integer indicating success or error code
*/
int memoryManager(char *args);

/**
   The initializeHeap function initailizes the Heap to a size defined in kmain
*/
void initializeHeap(u32int size);

/**
   The initLists function initializes the Free and Allocated lists to the size defined in kmain.
*/
void initLists(u32int size);

/**
   The allocate function is used to allocated new memory control blockes from the Heap using the first-fit method.
*/
u32int allocate(u32int size);

/**
   The freeMemory function is used to free a particular block of memory that was previously allocated by removing it from the allocated list and placing it into the free list.
*/
u32int freeMemory(u32int ba);

/**
   The merge function checks to see if there are any free blocks adjacent to one another. If so it will then merge the adjacent blocks into one large free block.
*/
void merge();

/**
   The showAllocated function traverses the Allocated list and shows the address of each block in the list along with its corresponding size.
*/
void showAllocated();

/**
   The showFree function traverses the Free list and displays the address of each block in the list along with its corresponding size.
*/
void showFree();

/**
   The isEmpty function will return true of false based on whether the heap contains ONLY free memory.
*/
int isEmpty();
#endif
