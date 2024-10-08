#include "globpcb.h"
#include <stdlib.h>
#include "pcb.h"


GlobPCB::GlobPCB(int dim)
{
   dimension = ( dim < 1 ? 10 : dim);
   first = add_new();
};

Page* GlobPCB::add_new()
{
   Page *temp = new Page();
   if (temp)
   {
      temp->pg = new PCB*[dimension];
      if (temp->pg)
      {
	 temp->next = NULL;
      } else temp = NULL;
   };
   return temp;
};

GlobPCB::~GlobPCB()
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


void GlobPCB::put(ID id, PCB* pcb)
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

   if (temp)  temp->pg[id] = pcb;
};
  


PCB* GlobPCB::get(ID id)
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
  