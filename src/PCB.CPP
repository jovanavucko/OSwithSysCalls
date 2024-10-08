#include "pcb.h"
#include "lock.h"
#include "schedule.h"
#include "kernel.h"
#include "thread.h"
#include "queue.h"
#include <stdlib.h>
#include <dos.h>

#include "globpcb.h"


ID PCB::GlobalID = 0;   //Initialization of GlobalID


PCB::PCB(StackSize size, Time timeSlice)
{
   myID = GlobalID++;
   myStackSize = (size > MAX_STACK_SIZE ? MAX_STACK_SIZE : size);
   myTimeSlice = timeSlice;
   myStack = NULL;
   myState = NEW;
   myQueue = new Queue();
   slicesPassed = 0;
   Kernel::allPCBs->put(myID, this);
};


void PCB::createStack(Thread *w)
{

   _LOCK;

   static volatile unsigned newSP, newSS, newIP, newCS, oldSP, oldSS, temp;
   static volatile unsigned t1,t2;

   myStack = new char [myStackSize];
   mySP = FP_OFF(myStack + myStackSize - 1);  //stek raste NA DOLE
   mySS = FP_SEG(myStack + myStackSize - 1);

   newSP = mySP;
   newSS = mySS;

   asm{
      mov temp, ax
      mov ax, word ptr [bp+0x0C]
      mov t1, ax
      mov ax, word ptr [bp+0x0A]
      mov t2, ax
      };

   newIP = FP_OFF (&(Thread::wrapper)) ;   // Pocetna adresa f-ije
   newCS = FP_SEG (&(Thread::wrapper)) ;   // CodeSegment
   asm{
      mov oldSP,sp   //cuvamo stari stek pointer u oldSP
      mov oldSS,ss   //cuvamo stari stek segment u oldSS
      mov sp, newSP  //postavlja se novi stek pointer
      mov ss, newSS  //postavlja se novi stek segment


      mov ax,word ptr t1
      push ax
      mov ax,word ptr t2
      push ax

      mov ax, newCS  // za pravilno dohvatanje argumenata?
      push ax
      mov ax, newIP  
      push ax

      pushf          //flag register na stek
      pop ax
      or ax, 0x0200  // bit I na 1, jer smo imali LOCK
      push ax        // vracamo flag registar na stek
      mov ax, newCS  // stavljamo cs na stek
      push ax
      mov ax, newIP  // stavljamo pc na stek
      push ax
      mov ax, temp   //cuvamo registre na stek
      push ax
      push bx
      push cx
      push dx
      push es
      push ds
      push si
      push di
      push bp
      mov newSP, sp  //novi SP ukazuje na vrh novonapakovanog steka
      mov newSS, ss  // isto za stek segment
      mov sp, oldSP  // vracamo stari stek
      mov ss, oldSS  //i ss
   }
   mySP = newSP;     //azuriramo SP i SS PCBA
   mySS = newSS;

   myState = READY;

   _UNLOCK;
};




PCB::~PCB()
{

   if (myQueue != NULL) delete myQueue;
   if (myStack != NULL) delete myStack;

};
