#include "kersem.h"
#include "scheduler.h"
#include "kernel.h"
#include "lock.h"
#include "pcb.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "globsem.h"


ID KernelSem::GlobalSemID = 0;


KernelSem::KernelSem(int init)
{
   value = init;
   semQueue = new Queue();

   Kernel::allKernelSems->put(GlobalSemID++,this);
};



KernelSem::~KernelSem()
{
   _LOCK;
   while (!(semQueue->empty()))
   {
      PCB *temp = semQueue->get();
      temp->myState = READY;
      Scheduler::put(temp);
      temp = NULL;
   };
   _UNLOCK;
};



void KernelSem::wait()
{
   _LOCK;
   value--;
   if (value<0)
   {
      Kernel::running->myState = BLOCKED;
      semQueue->put(Kernel::running);
   };
   _UNLOCK;
};



void KernelSem::signal()
{
   _LOCK;
   value++;
   if (value<1)
   {
        PCB *temp = semQueue->get();
	temp->myState = READY;
	Scheduler::put(temp);
   };
   _UNLOCK;
};