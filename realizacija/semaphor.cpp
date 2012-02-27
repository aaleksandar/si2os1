// file: semaphor.cpp
// author: Aleksandar Abu-Samra

#include "semaphor.h"
#include "ksem.h"
#include "kernel.h"
#include "pcb.h"

Semaphore::Semaphore(int init) {
	LOCK;
	myImpl = new KernelSem(init);
	UNLOCK;
}

Semaphore::~Semaphore() {
	LOCK;
	delete myImpl;
	UNLOCK;
}

int Semaphore::wait(Time maxTimeToWait) {
	LOCK;
	int ret = myImpl->wait(maxTimeToWait);
	UNLOCK;
	if (0 != semPreempt) Kernel::dispatch();
	return ret;
}

void Semaphore::signal() {
	LOCK;
	
	myImpl->signal();
	UNLOCK;
	if (0 != semPreempt) Kernel::dispatch();
}

int Semaphore::val() const {
	// if (0 != semPreempt) Kernel::dispatch(); // TODO i ovde?
	return myImpl->getVal();
}