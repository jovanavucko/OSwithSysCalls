#ifndef _KERNELEV_H_
#define _KERNELEV_H_

#include "event.h"

class PCB;

class KernelEv{
   public: 
      KernelEv(IVTNo ivtno);
      ~KernelEv();
      
      void wait();
      void signal();

      void set_mark() { mark = 1; }

   protected:
      friend class Event;
      friend class IVTEntry;
      friend class Kernel;
 
   private:
      IVTNo myIVTNo;
      int mark;


      static ID GlobalEventID;
      PCB *waitingPCB;
      IVTEntry *myEntry;
};

#endif