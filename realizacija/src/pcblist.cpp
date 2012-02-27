// file: pcblist.cpp
// author: Aleksandar Abu-Samra

#include <string.h>
#include "pcblist.h"
#include "pcb.h"
#include "kernel.h"
#include "kev.h"

PCBList::PCBList() {
	first = NULL;
}

PCBList::~PCBList() { // TOOPT: sa currrent pokazivacem
	while (first) {
		Elem *oldElem = first;
		first = first->next;

		delete oldElem;
	}
}

void PCBList::insert(PCB *newPtr) {
	Elem *newElem = new Elem (newPtr, first); // ubacuje element na pocetak liste
	first = newElem;
}

PCB* PCBList::getFirst () {
	if (NULL == first) return NULL;
	PCB* res = first->ptr;
	return res;
}

PCB* PCBList::getAndRemoveFirst() {
	if (NULL == first) return NULL;
	
	PCB* res = first->ptr;
	Elem* old = first;
	first = first->next;
	delete old;

	return res;
}

void PCBList::remove(PCB *removePtr) {
	if (NULL == first) return;

	Elem *prev = NULL, *i = first;

	while (i->ptr != removePtr) {
		prev = i;
		i = i->next;
		if (NULL == i) return;
	}

	if (NULL != prev) prev->next = i->next;
	else first = i->next;

	delete i;
}

PCB* PCBList::getById(ID id) { // TOOPT
	Elem *i = first;

	while (NULL != i) {
		if (i->ptr->getId() == id) return i->ptr;
		i = i->next;
	}

	return NULL;
}

ID PCBList::getFreeId() { // TOOPT
	ID id = 0;
	while (this->getById(id) != 0) id++;
	return id;
}

ID PCBList::getIdByName(TName name) { // TOOPT
	Elem *i = first;
	
	while (NULL != i) {
		if (0 == strcmp(i->ptr->getName(), name)) return i->ptr->getId();
		i = i->next;
	}

	return NULL;
}

void PCBList::handleTimedBlock() {
	if (NULL == first) return;

	Elem *i = first;

	while (i) {
		i->ptr->timeToWait--;
		Elem *next = i->next; // save next
		
		if (i->ptr->timeToWait <= 0) {
			
			i->ptr->wakeUp();
			i->ptr->deblockType = 2;
			
			remove(i->ptr);		
		}
		
		i = next;
		if (NULL == i) return;
	}
}

int PCBList::hasBlocked() {
	if (NULL != first) {	
		Elem *i = first;
		
		while (i) {
			if (i->ptr->state == PCB::BLOCKED) {
				return 1;
			}
			i = i->next;
		}
	}
	return 0;
}

void PCBList::signalAllEvents(IVTNo ivtNo) {
	Elem *i = first;
	
	while (NULL != i) {
		KernelEv *ke = (KernelEv*)i->ptr;
		if (ke->ivtNo == ivtNo)	ke->signalEv();
		i = i->next;
	}
}
