// file: ksem.h
// author: Aleksandar Abu-Samra

#ifndef _ksem_h_
#define _ksem_h_

#include "semaphor.h"
#include "pcblist.h"

class KernelSem {
	PCBList *blockedList;
	
protected:
	int val;
	
	void block(Time);
	void unblock();
	
public:
	KernelSem(int);
	~KernelSem();
	
	int wait(Time maxTimeToWait = 0);
	void signal();

	int getVal();
	
	friend class PCB;
};

#endif