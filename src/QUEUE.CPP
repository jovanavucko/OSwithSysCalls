#include "queue.h"
#include "lock.h"
#include <stdlib.h>




Queue::Queue()
{
   first = last = NULL;
};



Queue::~Queue()
{
   while (first) {
      Element *temp = first;
      first = first->next;
      delete temp;
   }
   first = last = NULL;
};


int Queue::empty() const { return (first == NULL); }


void Queue::put(PCB *p)
{
   Element *temp = new Element();
   temp->pcb = p;
   temp->next = NULL;

   if ( first == NULL ){
      first  = temp;
      last = first;
   } else {
	  last->next = temp;
	  last = temp;
   };
};



PCB* Queue::get()
{
   if (first == NULL) return NULL;  // ako nema elemenata u queue

   Element *temp = first;
   if (first == last) { first = NULL; last = NULL; }  // ako postoji samo jedan element
       else { first = first->next; }
   PCB *newPCB = temp->pcb;
   temp->pcb = NULL;
   temp->next = NULL;
   delete temp;

   return newPCB;
};