// file: idle.h
// author: Aleksandar Abu-Samra

#ifndef _idle_h_
#define _idle_h_

#include "thread.h"

class IdleThread : public Thread {

protected:
	void run();
	
public: 
	IdleThread();
	~IdleThread();
};

#endif