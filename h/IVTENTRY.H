#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_

#include "event.h"
#include "makros.h"

class KernelEv;

class IVTEntry{
   public:
      IVTEntry(IVTNo ivtNo, void interrupt (*newr)(...));
      ~IVTEntry();

      static IVTEntry *getEntry(IVTNo ivtno);
      void setEntry(KernelEv *kerevent);

      void interrupt (*oldRoutine)(...);
      KernelEv *myKernelEv;

 private:

      static IVTEntry **allEntries;
      void interrupt (*newRoutine)(...);
      IVTNo myNo;

      friend class Kernel;
      friend class KernelEv;
};


#endif