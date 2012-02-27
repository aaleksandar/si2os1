// file: ksem.cpp
// author: Aleksandar Abu-Samra

#include "ksem.h"
#include "pcblist.h"
#include "kernel.h"
#include "pcb.h"
#include "scheduler.h"

KernelSem::KernelSem(int init) {
	val = init;
	blockedList = new PCBList();
}

KernelSem::~KernelSem() {
	LOCK;
	
	// just in case...
	while (val++ < 0) {
		unblock();
	}
	// TOFIX ??
	
	delete blockedList;
	UNLOCK;
}

void KernelSem::block(Time maxTimeToWait) {
	LOCK;
	Kernel::running->state = PCB::BLOCKED;
	
	Kernel::running->blockingSem = this;
	blockedList->insert(Kernel::running);
	
	if (maxTimeToWait > 0) {
		Kernel::running->timeToWait = maxTimeToWait;
		Kernel::timedList->insert(Kernel::running);
	}
	Kernel::dispatch();
	
	UNLOCK;
}

void KernelSem::unblock() {
	LOCK;
	
	PCB* lucky = blockedList->getAndRemoveFirst();
	if(lucky) {
		lucky->state = PCB::READY;
		lucky->blockingSem = NULL;
		lucky->deblockType = 1; // HERE?
		if (lucky->timeToWait > 0) Kernel::timedList->remove(lucky);
		
		Scheduler::put(lucky);
	}
	
	UNLOCK;
}

int KernelSem::wait(Time maxTimeToWait) {
	LOCK;
	if (Kernel::running->state == PCB::RUNNING || Kernel::running->state == PCB::READY) { // needed?
		if (--val < 0) {
			block(maxTimeToWait);
		}
		else Kernel::running->deblockType = 1;
	}
	
	UNLOCK;
	return Kernel::running->deblockType;
}

void KernelSem::signal() {
	LOCK;
	if (val++ < 0) unblock();
	UNLOCK;
}

int KernelSem::getVal() {
	return val;
}