// file: pcb.h
// author: Aleksandar Abu-Samra

#ifndef _pcb_h_
#define _pcb_h_

#include "thread.h"
#include "ksem.h"
class KernelEv;

class PCB {
	ID id;
	TName name;
	StackSize stackSize;
	Time timeSlice;
	Thread *myThread;
	
	PCB *myParent;
	int offspring; // DEL ?
	
	int deblockType;
	int timeToWait;
	
	KernelSem *blockingSem;
	KernelSem *offspringSem;
	KernelSem *selfSem;
	
	KernelEv *blockingEv;

	enum State { NEW, READY, RUNNING, BLOCKED, TERMINATED } state;
	struct Context { unsigned sp, ss, *stack; } context;
	
	static void wrapper();
	
	void allocateContext();
	
	void emptySelfSem();
	
public:
	PCB (TName name, StackSize stackSize, Time timeSlice, Thread *myThread);
	~PCB();
	
	ID getId();
	TName getName();
	Thread* getMyThread();
	
	void start();
	int waitForChildren(PCB*);
	int waitToComplete(PCB*);
	int wakeUp();
	
protected:
	friend class Thread;
	friend class IdleThread;
	friend class PCBList;
	friend class Child;
	friend class Producer;
	friend class Timer;
	friend class Kernel;
	friend class KernelSem;
	friend class KernelEv;
	friend class Queue;
	
	friend class TestProducer;
	friend class Child;
};

#endif