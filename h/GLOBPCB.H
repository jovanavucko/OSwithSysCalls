#ifndef _glob_pcb_
#define _glob_pcb_

#include "thread.h"

class PCB;

class GlobPCB{
   public:
      GlobPCB(int a = 10);
      ~GlobPCB();
      void put(ID id, PCB* pcb);
      PCB* get(ID id);
   private:
      struct Page{
         PCB** pg;
         Page* next;
         };
      Page *first;
      Page* add_new();
      int dimension;
};

#endif