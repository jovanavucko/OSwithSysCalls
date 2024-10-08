#ifndef _EVENT_H_
#define _EVENT_H_


typedef unsigned char IVTNo;

#include "thread.h"
#include "makros.h"




class KernelEv;

class Event{
   public:
      Event(IVTNo ivtNo);
      ~Event();
      
      void wait();

   protected:
      friend class KernelEv;
      void signal();  // can call KernelEv

   private:
      ID myEvID;
      
};

#endif