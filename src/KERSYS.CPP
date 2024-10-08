#include "kernel.h"
#include "pcb.h"

#include "thread.h"

#include "queue.h"
#include "kersem.h"

#include "schedule.h"
#include "lock.h"
#include "idle.h"
#include "sysdet.h"
#include <stdlib.h>
#include <dos.h>

#include "globpcb.h"
#include "globsem.h"
#include "globev.h"


void interrupt Kernel::detectCall(...)
{
   static unsigned int t1,t2,t3,t4;
   static SysDetect *source;

   asm{
      mov t3,cx
      mov t4,dx
   };
   asm{
      mov t1, sp
      mov t2, ss
   }
      running->mySP = t1;
      running->mySS = t2;
      t1 = basePCB->mySP;
      t2 = basePCB->mySS;
   asm{
      mov sp, t1
      mov ss, t2
   };
   source = (SysDetect*)(MK_FP(t3,t4));

   flagLock

   asm sti;

//-------------------------------------------------------------

   SysCall(source);

//----------------------------------------------------------------

   asm cli;

   flagEvUnlock

   flagUnlock

   t1 = running->mySP;
   t2 = running->mySS;
   asm{
      mov sp, t1
      mov ss, t2
   };
};



void Kernel::SYS_CALL(SysDetect *call)
{
   asm{
      mov cx, [bp + 0x08]
      mov dx, [bp + 0x06]
      };
   geninterrupt(0x60);
};




void Kernel::post_wrapper()
{
   _LOCK;

   running->myState = FINISHED;
   while (!(running->myQueue->empty()))
   {
	PCB *temp = running->myQueue->get();
	temp->myState = READY;
	Scheduler::put(temp);
   };

   _UNLOCK;
};



void Kernel::SysCall(SysDetect *source)
{
   
  switch (source->functionID)
   {
      case 0: {
	 explDispatch = 1;
	 break;
	 }  // dispatch

      case 1: {
	 new PCB(source->size, source->tSlice);
	 source->callID = PCB::GlobalID - 1;
	 break;
	 }  // thread constructor


      case 2: {
	 if (allPCBs->get(source->callID))
	 {
	    delete allPCBs->get(source->callID);
            allPCBs->put(source->callID, NULL);
	 };
	 break;
	 }  // thread desstructor


      case 3: {
	_LOCK;
	PCB *temp = allPCBs->get(source->callID);
	if ( temp != NULL)
	{
	   if ((temp->myState == FINISHED ) || (temp == running ) ||
	   (temp == allPCBs->get(idleThread->myID)) || (temp == allPCBs->get(startingThread->myID)))
	   { }
	   else {
	      running->myState = BLOCKED;
	      temp->myQueue->put(running);
	      explDispatch = 1;
		};
	};
	_UNLOCK;
	break;
	}  // thread wait to complete

      case 4: {
	 PCB *temp = allPCBs->get(source->callID);
	 if ( temp != NULL)
	 {
	    temp->createStack(source->caller);
	    if (source != idleThread)
	    Scheduler::put(temp);
	 }
	 break;
	 }  // thread start



	case  5: {
	   post_wrapper();
	   explDispatch = 1;
           delete source;
	   break;
	   }  // thread post-run


//****************************************************************

       case  6: {  // OK

	  _LOCK;

	new KernelEv(source->ivtno);
	source->callID = KernelEv::GlobalEventID - 1;

	   _UNLOCK;

	  break;
	  }  // event constructor


       case  7: {  // OK

	  _LOCK;

	  KernelEv *temp = allKernelEvs->get(source->callID);

       if ( temp )
       {
	  delete temp;
	  allKernelEvs->put(source->callID,NULL);
       };

	 _UNLOCK;

	  break;
	  }  // event destructor


       case  8: {

	  KernelEv *temp = allKernelEvs->get(source->callID);

       if ( temp )
	  {

	     temp->signal();
	  }
	  break;
	  }  // event signal


       case  9: {       // OK

	  KernelEv *temp = allKernelEvs->get(source->callID);

	  if ( temp != NULL )
	  {
	     temp->wait();
	     explDispatch = 1;
	  }
	  break;
	  }  // event wait

//*********************************************************************

       case  10: {
	  new KernelSem(source->val);
	  source->callID = KernelSem::GlobalSemID - 1;
	  break;
	  }  // sem constructor


       case  11: {
	  if (allKernelSems->get(source->callID))
	  {
	    delete allKernelSems->get(source->callID);
	    allKernelSems->put(source->callID, NULL);
	  };
	  break;
	  }  // sem destructor



       case  12: {
	 KernelSem *temp = allKernelSems->get(source->callID);
	 if ( temp )
	  {
	     temp->wait();
	     if (running->myState == BLOCKED)
	       explDispatch = 1;
	  }
	  break;
	  }  // sem wait



       case  13: {
	 KernelSem *temp = allKernelSems->get(source->callID);
	 if ( temp )
	  {
	     temp->signal();
	  };
	  break;
	  }  // sem signal




       case  14: {
	 KernelSem *temp = allKernelSems->get(source->callID);
	 if ( temp )
	     { source->val = temp->val(); }
	     else source->val = 0;
	  break;
	  }  // sem val



       default: { break; }

   };
};







