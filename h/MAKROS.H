#ifndef _makros_h_
#define _makros_h_

#include "ivtentry.h"
#include "kernelev.h"
#include "lock.h"
/*
#define PREPAREENTRY(a,b)\
   void interrupt rout_##a(...)\
   {\
      if (b) (IVTEntry::getEntry(a))->oldRoutine();\
      IVTEntry::getEntry(a)->myKernelEv->signal();\
      if (Kernel::lockFlag)\
	 { dispatch(); }\
	  else { Kernel::explDispatch = 1; }\
   };\
   IVTEntry* IVT_##a = new IVTEntry((IVTNo)(a),rout_##a);
*/

#define PREPAREENTRY(a,b)\
   void interrupt rout_##a(...)\
   {\
      if (b) (IVTEntry::getEntry(a))->oldRoutine();\
      if (Kernel::lockFlag){\
      IVTEntry::getEntry(a)->myKernelEv->signal();\
      } else\
      {\
	 IVTEntry::getEntry(a)->myKernelEv->set_mark();\
	 Kernel::eventFlag = 1;\
      }\
   };\
   IVTEntry* IVT_##a = new IVTEntry((IVTNo)(a),rout_##a);

#endif