// file: idle.cpp
// author: Aleksandar Abu-Samra

#include "idle.h"
#include "pcb.h"

IdleThread::IdleThread() : Thread("idleThread") {
	myPCB->allocateContext();
}

IdleThread::~IdleThread() {
	myPCB->state = PCB::TERMINATED;
}

void IdleThread::run() {
	while(1);
}
