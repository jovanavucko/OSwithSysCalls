#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include "thread.h"

class KernelSem;

class Semaphore{
   public:
      Semaphore(int init = 1);
      virtual ~Semaphore();
      
      virtual void wait();
      virtual void signal();

      int val () const;  // Returns the current value of semaphore

   private:
      ID mySemID;
};

#endif