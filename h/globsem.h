#ifndef _glob_sem_
#define _glob_sem_

#include "thread.h"

class KernelSem;

class GlobSem{
   public:
      GlobSem(int a = 10);
      ~GlobSem();
      void put(ID, KernelSem*);
      KernelSem* get(ID id);
   private:
      struct Page{
         KernelSem** pg;
         Page* next;
         };
      Page *first;
      Page* add_new();
      int dimension;
};

#endif