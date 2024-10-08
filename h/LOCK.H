#ifndef _LOCK_H_
#define _LOCK_H_


#include "kernel.h"


#define _LOCK asm{\
		 pushf;\
		 cli;}
#define _UNLOCK asm{\
		   popf;}

#define flagLock Kernel::lockFlag=0;

#define flagUnlock Kernel::lockFlag=1;\
			    if (Kernel::explDispatch) Kernel::dispatch();


#define flagEvUnlock if (Kernel::eventFlag) Kernel::unmark();



#endif