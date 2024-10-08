#ifndef _KERNEL_H_
#define _KERNEL_H_


#include "thread.h"
#include "sysdet.h"
#include <iostream.h>
#include <dos.h>



class PCB;
class Idle;
class Thread;
class KernelSem;
class KernelEv;
class GlobPCB;
class GlobEv;
class GlobSem;

class Kernel{

   public:
      static void SYS_CALL(SysDetect*);

      static void init_Kernel();
      static void termin_Kernel();
      static volatile int explDispatch;
      static volatile int lockFlag;
      static volatile int eventFlag;

   private:
      static void interrupt timer_interruption(...);
      static void interrupt (*oldRoutine)(...);
      static void put_new_interrupt();
      static void remove_new_interrupt();

      static void unmark();

      static Thread *startingThread;
      static Idle *idleThread;
      static void post_wrapper();
      static PCB *running;
      static void dispatch();
	 
      static void interrupt detectCall(...);
      static void SysCall(SysDetect*);
      static void interrupt (*oldSys)(...);
      static PCB *basePCB;

      static GlobPCB *allPCBs;
      static GlobSem* allKernelSems;
      static GlobEv* allKernelEvs;

      friend class Thread;
      friend class PCB;
      friend class KernelEv;
      friend class KernelSem;
     

};

#endif