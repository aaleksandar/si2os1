// file: pcblist.h
// author: Aleksandar Abu-Samra

#ifndef _pcblist_h_
#define _pcblist_h_

#include <stdlib.h>
#include "thread.h"

class PCB;
class KernelEv;
typedef unsigned char IVTNo;

class PCBList {

	struct Elem {
		PCB *ptr;
		Elem *next;
		Elem(PCB *p, Elem *n = NULL) {
			ptr = p;
			next = n;
		}
	} *first;

public:

	PCBList();
	~PCBList();

	void insert(PCB*);
	
	PCB* getFirst();
	void remove(PCB *);
	PCB* getAndRemoveFirst();

	PCB* getById(ID);
	ID getFreeId();
	ID getIdByName(TName);
	
	void handleTimedBlock();
	int hasBlocked();
	
	void signalAllEvents(IVTNo);
};

#endif