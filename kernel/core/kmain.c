/*
  ----- kmain.c -----

  Description..: Kernel main. The first function called after
      the bootloader. Initialization of hardware, system
      structures, devices, and initial processes happens here.
*/

#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>

#include "modules/mpx_supt.h"
#include "modules/commhand.h"
#include "modules/queues.h"
#include "modules/processControl.h"
#include "modules/mcb.h"

void start_commhand_proc();
void start_idle_proc();

void kmain(void)
{
   extern uint32_t magic;
   // Uncomment if you want to access the multiboot header
   // extern void *mbd;
   // char *boot_loader_name = (char*)((long*)mbd)[16];

   // 0) Initialize Serial I/O and call mpx_init
   klogv("Starting MPX boot sequence...");
   klogv("Initialized serial I/O on COM1 device...");
   init_serial(COM1); //The COM1 variable is defined in /include/core/serial.h
   set_serial_in(COM1);
   set_serial_out(COM1);
   mpx_init(MODULE_R5);
   
   // 1) Check that the boot was successful and correct when using grub
   // Comment this when booting the kernel directly using QEMU, etc.
   if ( magic != 0x2BADB002 ){
     //kpanic("Boot was not error free. Halting.");
   }
   
   // 2) Descriptor Tables
   klogv("Initializing descriptor tables...");
   init_gdt();
   init_idt(); //interrupt descriptor table initialization
   init_irq(); //Intial interrupt handlers for first 32 irq lines
   sti(); //Enable interrupts

   // 4) Virtual Memory
   klogv("Initializing virtual memory...");
   init_paging(); //initializing paging ->allows use of virtual memory
   initializeHeap(50000); //initializing heap
   
   // 5) Call Commhand
   klogv("Transferring control to commhand...");
   init_queues(); //initialize process queues
   start_commhand_proc(); //create commhand process
   start_idle_proc(); //create idle process
   asm volatile ("int $60"); //yield to commhand & idle process
   //start_commhand();
   
   // 11) System Shutdown
   klogv("Starting system shutdown procedure...");
   
   /* Shutdown Procedure */
   klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
   hlt();
}


void start_commhand_proc(){

  CreatePCB("commhand", "0", "9");
  pcb *new_pcb = FindPCB("commhand");
  context *cp = (context *)(new_pcb->stacktop);
  memset(cp, 0, sizeof(context));
  cp->fs = 0x10;
  cp->gs = 0x10;
  cp->ds = 0x10;
  cp->es = 0x10;
  cp->cs = 0x8;
  cp->ebp = (u32int)(new_pcb->stack);
  cp->esp = (u32int)(new_pcb->stacktop);
  cp->eip = (u32int) start_commhand;
  cp->eflags = 0x202;

}

void start_idle_proc(){
  
  CreatePCB("idleprocess", "0", "9");
  pcb *new_pcb = FindPCB("idleprocess");
  context *cp = (context *)(new_pcb->stacktop);
  memset(cp, 0, sizeof(context));
  cp->fs = 0x10;
  cp->gs = 0x10;
  cp->ds = 0x10;
  cp->es = 0x10;
  cp->cs = 0x8;
  cp->ebp = (u32int)(new_pcb->stack);
  cp->esp = (u32int)(new_pcb->stacktop);
  cp->eip = (u32int) idle;
  cp->eflags = 0x202;
  
}

