#include "kernel.h"
#include "pcb.h"
#include "schedule.h"
#include "lock.h"
#include "ivtentry.h"
#include "idle.h"
#include <stdlib.h>
#include <dos.h>
#include "globpcb.h"
#include "globsem.h"
#include "globev.h"

// ulazi za prekide

#define IVTenter 0x08
#define sysEnter 0x60 // a moze biti i 0x61 ili 0x63

//inicijalizacija statickih pr.

void interrupt (*Kernel::oldSys)(...) = NULL;
PCB *Kernel::basePCB = NULL;
GlobPCB* Kernel::allPCBs = new GlobPCB(50);
GlobSem *Kernel::allKernelSems = new GlobSem(10);
GlobEv *Kernel::allKernelEvs = new GlobEv(10);

PCB* Kernel::running = NULL;
volatile int Kernel::explDispatch = 0;
volatile int Kernel::eventFlag = 0;
volatile int Kernel::lockFlag = 1;     // = 1  dozvoljen prekid
void interrupt (*Kernel::oldRoutine)(...) = NULL;
Thread* Kernel::startingThread = NULL;
Idle* Kernel::idleThread = NULL;





void Kernel::init_Kernel(){

  _LOCK;

   oldSys = getvect(sysEnter);
   setvect(sysEnter,detectCall);
   basePCB = new PCB(defaultStackSize,1);
   basePCB->myStack = new char[basePCB->myStackSize];
   basePCB->mySP = FP_OFF(basePCB->myStack + basePCB->myStackSize - 1);  //stek raste NA DOLE
   basePCB->mySS = FP_SEG(basePCB->myStack + basePCB->myStackSize - 1);


   put_new_interrupt();
   startingThread = new Thread(256,1);
   allPCBs->get(startingThread->myID)->myState = READY;
   running = allPCBs->get(startingThread->myID);
   idleThread = new Idle();
   idleThread->start();

   _UNLOCK;
};




void Kernel::put_new_interrupt(){
   _LOCK;
   Kernel::oldRoutine = getvect(IVTenter);  // argument je br ulaza u ivt
   setvect(IVTenter, timer_interruption);   // br ulaza, nova prekidna
   _UNLOCK;
};



void Kernel::termin_Kernel(){


   _LOCK;

   remove_new_interrupt();
   delete startingThread;
   delete idleThread;

   IVTEntry** temp = IVTEntry::allEntries;
   IVTEntry::allEntries = NULL;
   delete []temp;

   setvect(sysEnter, oldSys);
   delete basePCB;

   _UNLOCK;
};




void Kernel::remove_new_interrupt(){
   _LOCK;
   setvect(IVTenter, Kernel::oldRoutine);
   _UNLOCK;
};





 void Kernel::dispatch()
{
   _LOCK;

   if ((running->myState == READY) && ( running != allPCBs->get(idleThread->myID)) && (running != allPCBs->get(startingThread->myID)))
	 {
	    Scheduler::put((PCB*)running);
	 };
	 running = Scheduler::get();
	 if (running == NULL)
	 {
	    running = allPCBs->get(idleThread->myID);
	 };
   explDispatch = 0;
   running->slicesPassed = 0;

   _UNLOCK;

};



void interrupt Kernel::timer_interruption(...){  // ovo  ce biti nova prekidna rutina, locks?

  static volatile unsigned int tempSP, tempSS;
   static PCB *newPCB;

   tick();
   (*oldRoutine)();
   running->slicesPassed++;

   if ((running->myTimeSlice <=  running->slicesPassed) && (running->myTimeSlice != 0))
   {
      if (lockFlag)
      {
	 explDispatch = 0;
	 if ((running->myState == READY) && ( running != allPCBs->get(idleThread->myID)) && (running != allPCBs->get(startingThread->myID)))
	 {
	    Scheduler::put((PCB*)running);
	 };
	 newPCB = Scheduler::get();

	 if (newPCB == NULL)
	 {
	    newPCB = allPCBs->get(idleThread->myID);

	 };
   if (Kernel::running != newPCB) {
	asm{
	     mov tempSP, sp
	     mov tempSS, ss
	   }
	running->mySP = tempSP;
	running->mySS = tempSS;
	running = newPCB;
	tempSP = running->mySP;
	tempSS = running->mySS;
	asm{
	mov sp, tempSP
	mov ss, tempSS
	   }
	running->slicesPassed = 0;
   };
     } else explDispatch = 1;
   };
};


void Kernel::unmark()
{
   _LOCK;

   for (int i = 0; i< KernelEv::GlobalEventID; i++)
   {
      KernelEv *temp = allKernelEvs->get(i);
      if ((temp) && (temp->mark))
      {
	 temp->signal();
	 temp->mark = 0;
      };
   };
   eventFlag = 0;

   _UNLOCK;
};


