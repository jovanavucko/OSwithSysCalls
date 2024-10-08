#include "ivtentry.h"
#include <stdlib.h>
#include <dos.h>

IVTEntry** IVTEntry::allEntries = new IVTEntry*[256];


IVTEntry::IVTEntry(IVTNo ivtno, void interrupt (*newr)(...))
{
   myNo = ivtno;
   myKernelEv = NULL;
   newRoutine = newr;
   allEntries[(int)myNo] = this;
};



IVTEntry* IVTEntry::getEntry(IVTNo ivtno)
{
   IVTEntry *temp = allEntries[(int)ivtno];
   return temp;
};


void IVTEntry::setEntry(KernelEv *kerevent)
{
   myKernelEv = kerevent;
   oldRoutine = getvect(int(myNo));
   setvect((int)myNo,newRoutine);
};


IVTEntry::~IVTEntry()
{
   allEntries[(int)myNo] = NULL;
   setvect(int(myNo), oldRoutine);
};

