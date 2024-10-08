#ifndef _glob_ev_
#define _glob_ev_

#include "thread.h"

class KernelEv;

class GlobEv{
   public:
      GlobEv(int a = 10);
      ~GlobEv();
      void put(ID, KernelEv*);
      KernelEv* get(ID id);
   private:
      struct Page{
         KernelEv** pg;
         Page* next;
         };
      Page *first;
      Page* add_new();
      int dimension;
};

#endif