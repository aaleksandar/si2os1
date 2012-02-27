// file: ivtentry.cpp
// author: Aleksandar Abu-Samra

#include "ivtentry.h"
#include <dos.h>
#include "kev.h"
#include "kernel.h"

IVTEntry::IVTEntry(IVTNo ivtNo, TIntr eventRoutine) {
	LOCK;
	this->ivtNo = ivtNo;
	oldIntrR = getvect(ivtNo);
	setvect(ivtNo, eventRoutine);
	UNLOCK;
}

IVTEntry::~IVTEntry() {
	setvect(ivtNo, oldIntrR);
}

// signal all events
void IVTEntry::signalAll() {
	Kernel::blockingEvList->signalAllEvents(ivtNo);
}