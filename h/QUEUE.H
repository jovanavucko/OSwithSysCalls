#ifndef _QUEUE_H_
#define _QUEUE_H_

class PCB;

class Queue{
   private:
      struct Element{
             PCB *pcb;
	     Element *next;
      };
      Element *first, *last;

   public:
       Queue();
       ~Queue();
       int empty() const;
       void put(PCB*);
       PCB* get();
};

#endif