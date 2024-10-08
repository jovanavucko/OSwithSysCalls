#include "semaphor.h"
#include "kernel.h"
#include <stdlib.h>


Semaphore::Semaphore(int init)
{
   SysDetect *temp = new SysDetect();
   temp->functionID = 10;
   temp->val = init;

   Kernel::SYS_CALL(temp);

   mySemID = temp->callID;
   delete temp;
};


Semaphore::~Semaphore()
{
   SysDetect *temp = new SysDetect();
   temp->functionID = 11;
   temp->callID = mySemID;

   Kernel::SYS_CALL(temp);

   delete temp;
};


void Semaphore::wait()
{
   SysDetect *temp = new SysDetect();
   temp->functionID = 12;
   temp->callID = mySemID;

   Kernel::SYS_CALL(temp);

   delete temp;
};


void Semaphore::signal()
{
   SysDetect *temp = new SysDetect();
   temp->functionID = 13;
   temp->callID = mySemID;

   Kernel::SYS_CALL(temp);

   delete temp;
};



int Semaphore::val() const 
{
   SysDetect *temp = new SysDetect();
   temp->functionID = 14;
   temp->callID = mySemID;

   Kernel::SYS_CALL(temp);

   int v = temp->val;
   delete temp;
   return v;
};
