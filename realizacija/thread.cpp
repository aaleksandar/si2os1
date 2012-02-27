// file: thread.cpp
// author: Aleksandar Abu-Samra

#include "thread.h"
#include "kernel.h"
#include "pcb.h"
#include "schedule.h"

void Thread::start() {
	LOCK;
	myPCB->state = PCB::READY;
	myPCB->allocateContext();
	Scheduler::put(myPCB);
	UNLOCK;
}

int Thread::waitToComplete() {
	LOCK;
	int ret = myPCB->waitToComplete(myPCB);
	UNLOCK;
	return ret;
}

int Thread::wakeUp() {
	LOCK;
	int ret = myPCB->wakeUp();
	UNLOCK;
	return ret;
}

ID Thread::getId() {
	LOCK;
	ID id = myPCB->id;
	UNLOCK;
	return id;
}

TName Thread::getName() {
	LOCK;
	TName name = myPCB->name;
	UNLOCK;
	return name;
}

Thread* Thread::getThreadById(ID id) {
	LOCK;
	PCB *pcb =  Kernel::pcbList->getById(id);
	Thread* thread = pcb->getMyThread();
	UNLOCK;
	return thread;
}

ID Thread::getIdOf(TName name) { // ??? allocate new string
	LOCK;
	ID id = Kernel::pcbList->getIdByName(name);
	UNLOCK;
	return id;
}

Thread::~Thread() {
	LOCK;
	this->waitToComplete();
	Kernel::pcbList->remove(myPCB);
	delete myPCB;
	UNLOCK;
}

Thread::Thread (TName name, StackSize stackSize, Time timeSlice) {
	LOCK;
	myPCB = new PCB(name, stackSize, timeSlice, this);
	Kernel::pcbList->insert(myPCB);
	UNLOCK;
}

int Thread::waitForChildren() {
	LOCK;
	int ret = myPCB->waitForChildren(Kernel::running);
	UNLOCK;
	return ret;
}

void Thread::dispatch() {
	Kernel::dispatch();
}