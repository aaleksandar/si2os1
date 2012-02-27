// file: event.cpp
// author: Aleksandar Abu-Samra

#include "event.h"
#include "kernel.h"
#include "kev.h"

Event::Event(IVTNo ivtNo) {
	LOCK;
	myImpl = new KernelEv(ivtNo);
	UNLOCK;
}

Event::~Event() {
	delete myImpl;
}

int Event::wait(int maxTimeToWait) {
	LOCK;
	int ret = myImpl->waitEv(maxTimeToWait);
	UNLOCK;
	return ret;
}

void Event::signal() {
	LOCK;
	myImpl->signalEv();
	UNLOCK;
}

