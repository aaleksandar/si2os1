// file: event.h
// author: Aleksandar Abu-Samra

#ifndef _event_h_
#define _event_h_

#include "ivtentry.h"


#define PREPAREENTRY(no,callOld)          \
	void interrupt eventRt##no(...);        \
	IVTEntry ivtEntry##no(no,eventRt##no);  \
	void interrupt eventRt##no(...){        \
		ivtEntry##no.signalAll();             \
		if (callOld) ivtEntry##no.oldIntrR(); \
	}

typedef unsigned char IVTNo;

class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	
	int wait (int maxTimeToWait);
	void signal();
	
private:
	KernelEv* myImpl;
};

#endif