#ifndef _PCB_H_
#define _PCB_H_


#include "thread.h"
#define MAX_STACK_SIZE 16384

class Queue;

typedef enum {NEW, READY, BLOCKED, FINISHED } State;


class PCB{
  public:
    PCB(StackSize, Time);
    ~PCB();
    void createStack(Thread *w);

   private:
    Queue *myQueue;
    static ID GlobalID;

    ID myID;
    char *myStack;
    StackSize myStackSize;
    Time myTimeSlice;            //umnozak od 55
    unsigned int mySP,mySS;
    State myState;
    volatile Time slicesPassed;  // vreme proteklo u slices

    friend class Kernel;
    friend class KernelEv;
    friend class KernelSem;

};



#endif