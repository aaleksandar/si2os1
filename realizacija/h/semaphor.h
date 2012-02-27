// file: semaphor.h
// author: Aleksandar Abu-Samra

#ifndef _semaphor_h_
#define _semaphor_h_

extern int semPreempt;
typedef unsigned int Time;
class KernelSem;

class Semaphore {
public:
	Semaphore (int init=1);
	~Semaphore ();
	
	virtual int wait (Time maxTimeToWait);
	virtual void signal();
	
	int val () const;  // Returns the current value of the semaphore
private:
	KernelSem* myImpl;
};

#endif