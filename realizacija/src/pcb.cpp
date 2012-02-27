// file: pcb.cpp
// author: Aleksandar Abu-Samra

#include <stdio.h>
#include <string.h>
#include "pcb.h"
#include "kernel.h"
#include "pcblist.h"
#include <dos.h>
#include "scheduler.h"
#include "kev.h"

extern int semPreempt;

PCB::PCB (TName iName, StackSize iStackSize, Time iTimeSlice, Thread *iMyThread) {
	LOCK;
	
	id = Kernel::pcbList->getFreeId();
	
	// petljanje oko imena
	if (NULL == iName) { // name = thread + ID
		char buffer [11];
		int strSize = 7 + strlen(itoa(id,buffer,10));
		name = new char[strSize]; 
		sprintf(name, "%s%d", "thread", id);
	}
	else {
		name = new char[strlen(iName)+1];
		sprintf(name, "%s", iName);
	}			
	
	if (iStackSize > 65536) iStackSize = 65536; // threadu se prosledjuju bajti ili broj ulaza?
	stackSize = iStackSize;
	timeSlice = iTimeSlice;
	myThread = iMyThread;
	
	offspring = 0;
	timeToWait = 0;
	
	myParent = Kernel::running;
	Kernel::running->offspring++;
	
	offspringSem = NULL;
	selfSem = new KernelSem(0);
	
	deblockType = 1;
	blockingSem = NULL;
	
	blockingEv = NULL;
	
	state = NEW;
	
	UNLOCK;
}

PCB::~PCB () {
	LOCK;
	
	if (offspringSem) delete offspringSem;
	if (selfSem) delete selfSem;
	if (blockingSem) delete blockingSem;
	delete name;
	delete [] context.stack;
	
	UNLOCK;
}

ID PCB::getId() {
	return id;
}

TName PCB::getName() {
	return name;
}

Thread* PCB::getMyThread() {
	return myThread;
}

void PCB::wrapper() {
	Kernel::running->myThread->run();
	LOCK;
	
	if (Kernel::running->myParent) {
		Kernel::running->myParent->offspring--; // jedna briga manje
		if (Kernel::running->myParent->offspring < 1) Kernel::running->myParent->offspringSem->signal(); // ostao si bez dece matori...
	}

	Kernel::running->state = PCB::TERMINATED;
	Kernel::running->emptySelfSem();
	
	Kernel::dispatch();
	
	UNLOCK;
}

void PCB::emptySelfSem() {
	LOCK;
	int tmp = semPreempt; semPreempt = 0;
	while (selfSem->getVal()<0) selfSem->signal();
	semPreempt = tmp;
	UNLOCK;
}

void PCB::allocateContext() {
	LOCK;
	context.stack = new unsigned[stackSize];
	static unsigned tsp, tss, tip, tcs, oldss, oldsp;
	
	tsp = FP_OFF(context.stack + stackSize - 1);
	tss = context.ss = FP_SEG(context.stack + stackSize - 1);

	tip = FP_OFF(PCB::wrapper);
	tcs = FP_SEG(PCB::wrapper);

	asm{
		mov oldss, ss
		mov oldsp, sp

		mov ss, tss
		mov sp, tsp

		pushf
		pop ax
		or ax, 200h
		push ax // interrupt

		mov ax, tcs
		push ax
		mov ax, tip
		push ax // set PCB::wrapper

		mov ax, 0
		push ax
		push bx
		push cx
		push dx
		push es
		push ds
		push si
		push di
		push bp 

		mov tsp, sp
		mov tss, ss
		mov sp, oldsp
		mov ss, oldss
	}
	
	context.ss = tss;
	context.sp = tsp;
	
	UNLOCK;
}

int PCB::waitForChildren(PCB *pcb) {
	LOCK;
	
	if (pcb->offspring > 0) {
		if (offspringSem) delete offspringSem;
		offspringSem = new KernelSem(0);
		offspringSem->wait();

		UNLOCK;
		return 0;
	}
	
	UNLOCK;
	return 1; // no offspring
}

int PCB::waitToComplete(PCB *pcb) {
	LOCK;

	// handle loopback call
	if (pcb == Kernel::running) {
		UNLOCK;
		return 0;
	}
	
	if (pcb->state != PCB::TERMINATED) {
		if (pcb->state == PCB::BLOCKED) pcb->selfSem->val++; 
		
		pcb->selfSem->wait();
		UNLOCK;
		return 0;
	}

	UNLOCK;
	return 1;
}

int PCB::wakeUp() {
	LOCK;
	
	if (PCB::BLOCKED == this->state) {
		this->state = PCB::READY;
		this->deblockType = 0;
		
		if (this->blockingSem) {
			this->blockingSem->blockedList->remove(this); // nepotrebno?
			this->blockingSem->val++;
			this->blockingSem = NULL;
		}
		else if(this->blockingEv) {
//			this->blockingEv->val++;
			this->blockingEv = NULL;
		}
		// TOFIX!!! ^^
		Scheduler::put(this);
	
		UNLOCK;
		return 1;
	}
	
	UNLOCK;
	return 0;
}