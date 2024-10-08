#ifndef _KERNELSEM_H_
#define _KERNELSEM_H_

#include "thread.h"

class Queue;

class KernelSem{
   public:
      KernelSem(int init);  // =1 dolazi iz semaphore konstruktora
      ~KernelSem();
      
      void wait();
      void signal();
    
      int val() const { return value; }

   private:

      volatile int value;
      static ID GlobalSemID;
      Queue *semQueue;

      friend class Kernel;
   
};

#endif