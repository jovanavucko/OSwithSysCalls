#include "event.h"
#include "kernelev.h"
#include "lock.h"
#include "kernel.h"
#include <stdlib.h>


Event::Event(IVTNo ivtNo)
{
   SysDetect *temp = new SysDetect();
   temp->functionID = 6;
   temp->ivtno = ivtNo;

   Kernel::SYS_CALL(temp);

   myEvID = temp->callID;
   delete temp;


};


Event::~Event()
{
   SysDetect *temp = new SysDetect();
   temp->functionID = 7;
   temp->callID = myEvID;


   Kernel::SYS_CALL(temp);

   delete temp;
};



void Event::wait()
{
   SysDetect *temp = new SysDetect();
   temp->functionID = 9;
   temp->callID = myEvID;

   Kernel::SYS_CALL(temp);

   delete temp;
};



void Event::signal()
{
   SysDetect *temp = new SysDetect();
   temp->functionID = 8;
   temp->callID = myEvID;

   Kernel::SYS_CALL(temp);

   delete temp;
};
