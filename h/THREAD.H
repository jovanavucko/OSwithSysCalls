#ifndef _THREAD_H_
#define _THREAD_H_


typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;

typedef unsigned int Time;  //time, x 55ms
const Time defaultTimeSlice = 2;  // default = 2*55ms
typedef int ID;


class PCB;  //Kernel's implementation of a user's thread

class Thread{
  public:
    void start();
    void waitToComplete();
    virtual ~Thread();

  protected:
    friend class Kernel;
    friend class PCB;

    Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
    virtual void run() {};

  private:
    ID myID;
    static void wrapper(Thread* running);

};

  extern void tick();
  void dispatch();

#endif