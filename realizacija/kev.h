// file: kev.h
// author: Aleksandar Abu-Samra

#ifndef _kev_h_
#define _kev_h_

#include "pcblist.h"
#include "event.h"

class PCB;


class KernelEv { //: public KernelSem {
	PCB *owner;
	int interruptOccurred;

	PCBList *blockedList;
public: 
	IVTNo ivtNo;

	KernelEv(IVTNo);
	~KernelEv();
	
	int waitEv(Time);
	void signalEv();
	
	friend class IVTEntry;
	friend class PCBList;
};

#endif