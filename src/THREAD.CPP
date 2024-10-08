#include "thread.h"
#include "kernel.h"
#include "sysdet.h"
#include <stdlib.h>



Thread::Thread (StackSize stackSize, Time timeSlice)
{
   SysDetect *temp = new SysDetect();
   temp->functionID = 1;
   temp->size = stackSize;
   temp->tSlice = timeSlice;

   Kernel::SYS_CALL(temp);

   myID = temp->callID;
   delete temp;
};



Thread::~Thread()
{
   waitToComplete();

   SysDetect *temp = new SysDetect();
   temp->functionID = 2;
   temp->callID = myID;

   Kernel::SYS_CALL(temp);

   delete temp;
};



void Thread::start()
{
   SysDetect *temp = new SysDetect();
   temp->functionID = 4;
   temp->callID = myID;
   temp->caller = this;


   Kernel::SYS_CALL(temp);

   delete temp;
};



void Thread::waitToComplete()   //DONE
{
   SysDetect *temp = new SysDetect();
   temp->functionID = 3;
   temp->callID = myID;

   Kernel::SYS_CALL(temp);

   delete temp;
};



void Thread::wrapper(Thread *running)
{
   running->run();

   SysDetect *temp = new SysDetect();
   temp->functionID = 5;

   Kernel::SYS_CALL(temp);
};



void dispatch()
{
   static SysDetect *temp = new SysDetect();
   temp->functionID = 0;

   Kernel::SYS_CALL(temp);

};