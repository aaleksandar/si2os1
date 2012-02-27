// file: kernel.h
// author: Aleksandar Abu-Samra

#ifndef _kernel_h_
#define _kernel_h_

#include "pcblist.h"
#include "thread.h"
#include "timer.h"
#include "idle.h"

#define LOCK asm{ pushf; cli }
#define UNLOCK asm{ popf; }

//#define LOCK asm cli
//#define UNLOCK asm sti

#define INTR1 0x08
#define INTR2 0x1C

enum Bool { FALSE, TRUE };

class Kernel {
public:
	static PCBList *pcbList;
	static PCBList *timedList;
	static PCB *running;
	static Timer *timer;
	static Thread *backToKernel;
	static IdleThread *idleThread;
	
	static PCBList *blockingEvList; // srecno mi bilo sa cast-ovanjem
	
	static int begin(int, char**);
	static void end();
	
	static void interrupt dispatch(...);
	//static void ispis(); // dbg tool
};

#endif