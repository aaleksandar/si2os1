// file: kev.cpp
// author: Aleksandar Abu-Samra

#include "kev.h"
#include "kernel.h"
#include "pcb.h"
#include "pcblist.h"

#include "SCHEDULE.H"

KernelEv::KernelEv(IVTNo ivtNo){
	Kernel::blockingEvList->insert((PCB*)this);
	owner = Kernel::running;
	this->ivtNo = ivtNo;
	interruptOccurred = 0;
	
	blockedList = new PCBList();
}

KernelEv::~KernelEv(){
	LOCK;
//	if(val < 0) owner->myThread->wakeUp();
	Kernel::blockingEvList->remove((PCB*)this);
	delete blockedList;
	UNLOCK;
}

int KernelEv::waitEv(Time maxTimeToWait){

	if (owner != Kernel::running) return -1; // running must be owner
	LOCK;
	
	if (interruptOccurred) {
		interruptOccurred = 0;
		Kernel::running->deblockType = 1;
	}
	else {
		Kernel::running->state = PCB::BLOCKED;
		Kernel::running->blockingEv = this;
		blockedList->insert(Kernel::running);
		if(maxTimeToWait > 0){
			Kernel::running->timeToWait = maxTimeToWait;
			Kernel::timedList->insert(Kernel::running);
		}
		Kernel::dispatch();
	}
	
	UNLOCK;
	return Kernel::running->deblockType;
}

void KernelEv::signalEv(){
// poziv uvek iz prekidne rutine, autolock
	PCB* lucky = blockedList->getAndRemoveFirst();
	
	if (NULL != lucky) {
		if(lucky->timeToWait > 0) Kernel::timedList->remove(lucky);
		lucky->state = PCB::READY;
		lucky->deblockType = 1;
		lucky->blockingEv = NULL;
		Scheduler::put(lucky);
	}
	else interruptOccurred = 1;
}