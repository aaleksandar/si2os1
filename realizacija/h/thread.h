// file: thread.h
// author: Aleksandar Abu-Samra

#ifndef _thread_h_
#define _thread_h_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;
typedef char *TName;

class PCB; // Kernel's implementation of a user's thread

class Thread {
public:
	void start();
	int waitToComplete(); //return: 1 – completed, 0 – woken up
	int wakeUp(); //return: 1 – deblocked, 0 – no effect
	
	ID getId();
	TName getName();
	static Thread *getThreadById(ID id);
	static ID getIdOf(TName name); //allocate new string

	void dispatch();

	virtual ~Thread();
	
protected:

	friend class PCB;
	friend class IdleThread;
	friend class Child;
	friend class TestProducer;
	friend class Producer;	
	friend class Kernel;
	Thread (TName name, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}
	int waitForChildren(); //return: 1 – finished, 0 – woken up
	
private:
	PCB *myPCB;
};

#endif