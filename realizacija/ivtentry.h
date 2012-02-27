// file: ivtentry.h
// author: Aleksandar Abu-Samra

#ifndef _ivtentry_h_
#define _ivtentry_h_

typedef unsigned char IVTNo;

typedef void interrupt (*TIntr)(...);

class IVTEntry {
	IVTNo ivtNo;
	
public:	
	IVTEntry(IVTNo, TIntr);
	~IVTEntry();
	
	TIntr oldIntrR;
	void signalAll();
};

#endif