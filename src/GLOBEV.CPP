#include "globev.h"
#include <stdlib.h>
#include "kernelev.h"
#include "lock.h"


GlobEv::GlobEv(int dim)
{
   dimension = ( dim < 1 ? 10 : dim);
   first = add_new();
};

Page* GlobEv::add_new()
{
   Page *temp = new Page();
   if (temp)
   {
      temp->pg = new KernelEv*[dimension];
      if (temp->pg)
      {
	 temp->next = NULL;
      } else temp = NULL;
   };
   return temp;
};

GlobEv::~GlobEv()
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


void GlobEv::put(ID id, KernelEv* ev)
{

   Page *temp = first, *last;
   while ((temp) && (id/dimension))
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

   if (temp)  temp->pg[id] = ev;

};
  


KernelEv* GlobEv::get(ID id)
{

   Page *temp = first;
   while ((temp) && (id / dimension))
   {
      temp = temp->next;
      id = id - dimension;
   };
   if ( temp == NULL) { return NULL; }
   else { return temp->pg[id]; };
};
  