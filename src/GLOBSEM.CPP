#include "globsem.h"
#include <stdlib.h>
#include "kersem.h"
#include "lock.h"


GlobSem::GlobSem(int dim)
{
   dimension = ( dim < 1 ? 10 : dim);
   first = add_new();
};

Page* GlobSem::add_new()
{
   Page *temp = new Page();
   if (temp)
   {
      temp->pg = new KernelSem*[dimension];
      if (temp->pg)
      {
	 temp->next = NULL;
      } else temp = NULL;
   };
   return temp;
};

GlobSem::~GlobSem()
{
   while (first)
   {
      Page* temp = first;
      first = first->next;
      for (int i = 0; i< dimension; i++)
         if (temp->pg[i]) delete temp->pg[i];
      delete []temp->pg;
      temp->next = NULL;
      
   };
};


void GlobSem::put(ID id, KernelSem* sem)
{
   _LOCK;

   Page *temp = first, *last;
   while ((temp != NULL) && (id/dimension))
   {
      last = temp;
      temp = temp->next;
      id = id - dimension;
   };
   if ( temp == NULL)
   {
      last->next = add_new();
      temp = last->next;
   };

   if (temp) temp->pg[id] = sem;

   _UNLOCK;
};
  


KernelSem* GlobSem::get(ID id)
{
   _LOCK;

   Page *temp = first;
   while ((temp) && (id / dimension))
   {
      temp = temp->next;
      id = id - dimension;
   };

   _UNLOCK;

   if ( temp == NULL) { return NULL; }
   else { return temp->pg[id]; };
};
  