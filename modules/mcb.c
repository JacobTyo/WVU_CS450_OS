#include "mcb.h"

list allocatedList;
list freeList;
u32int memoryStart;

int memoryManager(char *args){
  char *pflag = strtok(args, " "); //get first token (--whatever)
  char *parg1 = strtok(NULL, " "); //get second token (arg1)
  u32int size;
  if(strcmp(parg1,"") != 0){
    if(chkatoi(parg1) == 0) {
      size = atoi(parg1);
    } else {
      serial_println("Dude. arguements need to be ints");
      return 0;
    }
  } else {
    serial_println("You need an arguement bro.");
    return 0;
  }
  if(strcmp(pflag, "--initialize")==0) {
    initializeHeap(size);
    return 1;
  } else if(strcmp(pflag, "--free")==0) {
    freeMemory(size);
    return 1;
  } else if(strcmp(pflag, "--allocate")==0) {
    allocate(size);
    return 1;
  } else {
    return 0;
  }
}

void initializeHeap(u32int size) {
  memoryStart = kmalloc(size + sizeof(cmcb) + sizeof(lmcb));
  initLists(size);
}


void initLists(u32int size) {
  cmcb *firstCmcb = (cmcb*) memoryStart;
  firstCmcb->size = size;
  firstCmcb->allocated = 0;
  firstCmcb->beginningAddress = (int) memoryStart + sizeof(cmcb);
  lmcb *firstLmcb = (lmcb*) (memoryStart + sizeof(cmcb) + size);
  firstLmcb->allocated = 0;
  firstLmcb->size = size;
  firstCmcb->thisLmcb = firstLmcb;
  freeList.head = firstCmcb;
  allocatedList.head = NULL;
  allocatedList.head->nextcmcb = NULL;
  allocatedList.head->prevcmcb = NULL;
  freeList.head->nextcmcb = NULL;
  freeList.head->prevcmcb = NULL;
}

u32int allocate(u32int s) {
  cmcb *tmp = freeList.head;
  cmcb *createme;
  if (tmp == NULL){
    serial_println("There is no more free memory to be allocated");
    return 0;
  } else {
    while ((u32int) (tmp->size) < (u32int) (s + sizeof(cmcb) + sizeof(lmcb))){
      tmp = tmp->nextcmcb;
    }
    //at this point tmp holds the first mcb with ample space to allocate
    //now we need to move the head of existing block, and create head and tail of previous block
    //basically we need to put a newly created block into tmp and resize tmp
    int tmpishead = 0;
    if (tmp == freeList.head){
      tmpishead = 1;
    }

    u32int tmpsize = tmp->size;
    char tmpname[31];
    strcpy((char *) tmpname, (char *) tmp->name);
    u32int tmpal = tmp->allocated;
    u32int tmplmcb = (u32int) tmp->thisLmcb;
    u32int tmpnext = (u32int) tmp->nextcmcb;
    u32int tmpprev = (u32int) tmp->prevcmcb;

    createme = (cmcb *) tmp;
    createme->size = s;
    createme->allocated = 1;
    createme->beginningAddress = (u32int) tmp + sizeof(cmcb);
    lmcb *lcreateme =  (lmcb *)(createme->beginningAddress + s); //(lmcb *) (tmp + sizeof(cmcb) + s);
    lcreateme->allocated = 1;
    lcreateme->size = s;
    createme->thisLmcb = lcreateme;

    //resets the cmcb of the mcb which memory was stolen from
    cmcb *newhead = (cmcb *) ((u32int) lcreateme + sizeof(lmcb));
    newhead->size = tmpsize - s - sizeof(cmcb) - sizeof(lmcb);
    newhead->beginningAddress = ((u32int) lcreateme + sizeof(lmcb)) + sizeof(cmcb);
    strcpy((char *) newhead->name, (char *) tmpname);
    newhead->allocated = tmpal;
    newhead->thisLmcb = (lmcb *) tmplmcb;
    newhead->nextcmcb = (cmcb *) tmpnext;
    newhead->prevcmcb = (cmcb *) tmpprev;

    if (tmpishead == 1){
      freeList.head = newhead;
    }

    if (allocatedList.head == NULL){

      allocatedList.head = createme;

    } else {

      cmcb *palist = allocatedList.head;

      //check if createme must be placed before ahead, otherwise, place after
      while ((u32int) palist < (u32int) createme && palist->nextcmcb != NULL){
	palist = palist->nextcmcb;
      }

      if (palist < createme){
	//create me is last
	palist->nextcmcb = createme;
	createme->prevcmcb = palist;
      } else {
	//create me is being placed before palist
	createme->nextcmcb = palist;
	createme->prevcmcb = palist->prevcmcb;
	palist->prevcmcb = createme;
	if (createme->prevcmcb != NULL){
	  createme->prevcmcb->nextcmcb = createme;
	}
      }
    }
    return (u32int) createme->beginningAddress;
  }
}

u32int freeMemory(u32int ba) {
  cmcb *tempCmcb = allocatedList.head;
  cmcb *travCmcb = freeList.head;
  u32int returnme = ba;
  while (tempCmcb != NULL) {

    if (tempCmcb->beginningAddress == ba) {
      tempCmcb->allocated = 0;
      tempCmcb->thisLmcb->allocated = 0;

      //first remove from allocatedlist
      if (tempCmcb == allocatedList.head){
	allocatedList.head = tempCmcb->nextcmcb;
	tempCmcb->nextcmcb->prevcmcb = NULL;
      } else {
	if (tempCmcb->nextcmcb != NULL){
	  tempCmcb->prevcmcb->nextcmcb = tempCmcb->nextcmcb;
	  tempCmcb->nextcmcb->prevcmcb = tempCmcb->prevcmcb;
	} else {
	  tempCmcb->prevcmcb->nextcmcb = NULL;
	}
      }

      //now add to freelist
      if (freeList.head->beginningAddress > tempCmcb->beginningAddress) {

	//tempCmcb must be placed as the new freelist head
        freeList.head->prevcmcb = tempCmcb;
	tempCmcb->nextcmcb = freeList.head;
	freeList.head = tempCmcb;
	tempCmcb->prevcmcb = NULL;

      } else {

	while (travCmcb->beginningAddress < tempCmcb->beginningAddress && travCmcb->nextcmcb != NULL){
	  travCmcb = travCmcb->nextcmcb;
	}

	//place tempcmcb before travCmcb, unless travCmcb < tempcmcb and travCmcb->nextcmcb = null
	if (travCmcb->beginningAddress < tempCmcb->beginningAddress && travCmcb->nextcmcb == NULL){
	  //place after travcmcb
	  travCmcb->nextcmcb = tempCmcb;
	  tempCmcb->prevcmcb = travCmcb;
	  tempCmcb->nextcmcb = NULL;
	} else {
	  //place before travcmcb
	  tempCmcb->nextcmcb = travCmcb;
	  tempCmcb->prevcmcb = travCmcb->prevcmcb;
	  travCmcb->prevcmcb = tempCmcb;
	  tempCmcb->prevcmcb->nextcmcb = tempCmcb;
	}
      }
      break;
    }
    tempCmcb = tempCmcb->nextcmcb;
  }
  merge();
  return returnme;
}

void merge(){
  //merge everything to prevent weird segmentation in freelist
  cmcb *tempCmcb = freeList.head;
  while(tempCmcb->nextcmcb != NULL){

    if (((u32int) tempCmcb->nextcmcb) - ((u32int) tempCmcb->thisLmcb + sizeof(lmcb)) < 10){

      tempCmcb->size = tempCmcb->size + sizeof(lmcb) + sizeof(cmcb) + tempCmcb->nextcmcb->size;
      tempCmcb->thisLmcb = tempCmcb->nextcmcb->thisLmcb;
      tempCmcb->nextcmcb = tempCmcb->nextcmcb->nextcmcb;

      if(tempCmcb->nextcmcb != NULL){
        tempCmcb->nextcmcb->prevcmcb = tempCmcb;
      }
      //need to start back at the head to make sure all merges that were necessary happen
      tempCmcb = freeList.head;
      continue;
    }
    tempCmcb = tempCmcb->nextcmcb;
  }

}

void showAllocated() {
  if (allocatedList.head == NULL){
    serial_println("There is no allocated memory");
  } else {
    cmcb *tempCmcb = allocatedList.head;
    while (tempCmcb != NULL) {
      serial_print("Size: ");
      print_int(tempCmcb->size);
      serial_print("\n");
      serial_print("Begining Address: ");
      print_int(tempCmcb->beginningAddress);
      serial_print("\r\n");
      tempCmcb = tempCmcb->nextcmcb;
    }
  }
}

void showFree(){
  if (freeList.head == NULL){
    serial_println("There is not free memory");
  } else {
    cmcb *tempCmcb = freeList.head;
    while (tempCmcb != NULL) {
      serial_print("Size: ");
      print_int(tempCmcb->size);
      serial_print("\n");
      serial_print("Begining Address: ");
      print_int(tempCmcb->beginningAddress);
      serial_print("\n");
      tempCmcb = tempCmcb->nextcmcb;
    }
  }
}

int isEmpty(){
  if (allocatedList.head == NULL) {
    return 1;
  } else {
    return 0;
  }
}
