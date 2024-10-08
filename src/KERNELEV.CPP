#include "kernelev.h"
#include "scheduler.h"
#include "pcb.h"
#include "kernel.h"
#include "ivtentry.h"
#include <stdlib.h>
#include "lock.h"
#include "globev.h"


ID KernelEv::GlobalEventID = 0;


KernelEv::KernelEv(IVTNo ivtno)
{
   myIVTNo = ivtno;
   myEntry = IVTEntry::getEntry(ivtno);
   myEntry->setEntry(this);
   mark = 0;

   Kernel::allKernelEvs->put(GlobalEventID++, this);
};



KernelEv::~KernelEv()
{
   if (waitingPCB !=NULL)  // da li da je vratim ili izbrisem?
   {
      waitingPCB->myState = READY;
      Scheduler::put(waitingPCB);
      waitingPCB = NULL;
   };
   delete myEntry;
};




void KernelEv::wait()
{
//   _LOCK;

   Kernel::running->myState = BLOCKED;
   waitingPCB = Kernel::running;

//   _UNLOCK;
};



void KernelEv::signal()
{
//   _LOCK;

   if (waitingPCB !=NULL)
   {
      waitingPCB->myState = READY;
      Scheduler::put(waitingPCB);
      waitingPCB = NULL;
   };

//   _UNLOCK;
};
