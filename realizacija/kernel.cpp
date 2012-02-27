// file: kernel.cpp
// author: Aleksandar Abu-Samra

#include "kernel.h"
#include "pcblist.h"
#include "pcb.h"
#include "thread.h"
#include "timer.h"
#include "schedule.h"
#include "idle.h"

#include "umain.h"

extern long uptime;

PCBList* Kernel::pcbList = NULL;
PCBList* Kernel::timedList = NULL;
PCBList* Kernel::blockingEvList = NULL;
PCB* Kernel::running = NULL;
Timer* Kernel::timer = NULL;
Thread* Kernel::backToKernel = NULL;
IdleThread* Kernel::idleThread = NULL;

int Kernel::begin(int argc, char *argv[]) {
	LOCK;
	
	// initialization
	Kernel::pcbList = new PCBList();
	Kernel::timedList = new PCBList();
	Kernel::blockingEvList = new PCBList();
	
	Kernel::backToKernel = new Thread("backToKernel");	
	Kernel::running = Kernel::backToKernel->myPCB;
	
	Kernel::idleThread = new IdleThread();
	Kernel::timer = new Timer();
	// /initialization

	UMain *uMain = new UMain(argc, argv);
	
	Kernel::dispatch();
	
	UNLOCK;
	return uMain->ret;
}

void Kernel::end() {
	if (Kernel::pcbList) delete Kernel::pcbList;
	if (Kernel::timedList) delete Kernel::timedList;
	if (Kernel::backToKernel) delete Kernel::backToKernel;
	if (Kernel::timer) {
		uptime = Kernel::timer->uptime;
		delete Kernel::timer;
	}
}

void interrupt Kernel::dispatch(...) {
	static unsigned tss, tsp;
	
	asm{
		mov tss, ss
		mov tsp, sp
	}

	if (NULL != Kernel::running) {	
		Kernel::running->context.ss = tss;
		Kernel::running->context.sp = tsp;

		if (PCB::RUNNING == Kernel::running->state) {
			Kernel::running->state = PCB::READY;
			Scheduler::put(Kernel::running);
		}
	}

	Kernel::running = Scheduler::get();
	
	if (NULL != Kernel::running) { Kernel::running->state = PCB::RUNNING; }
	else if (Kernel::pcbList->hasBlocked()) {
		Kernel::running = Kernel::idleThread->myPCB; 
	}
	else { Kernel::running = Kernel::backToKernel->myPCB; }
	
	tss = Kernel::running->context.ss;
	tsp = Kernel::running->context.sp;
	asm{
		mov ss, tss
		mov sp, tsp
	}
	
	Timer::stopwatch = 0;
}