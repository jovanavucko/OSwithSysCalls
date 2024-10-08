#ifndef _sysdetect_h_
#define _sysdetect_h_

#include "thread.h"
#include "event.h"
#include <stdlib.h>


class SysDetect{
public:
      ID callID;  // sve osim za dispatch
      Thread* caller;  // thread konstruktor
      unsigned int functionID;  // opste
      StackSize size;  // thread konstruktor
      Time tSlice; // thread konstruktor
      IVTNo ivtno;
      int val;
      SysDetect() { caller = NULL; }
      };

#endif