//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    bounded.cpp
//   Date:    June 2007
//===========================================================================//
#include <stdlib.h>
#include "bounded.h"
#include "intLock.h"

BoundedBuffer::BoundedBuffer (unsigned size) : Size(size),
	mutexa(1), mutext(1), spaceAvailable(size), itemAvailable(0),
	head(0), tail(0) {
		buffer = new char[size];
		if (!buffer) exit(1);
	}
	
BoundedBuffer::~BoundedBuffer(){
	intLock
	delete [] buffer;
	intUnlock
}

int BoundedBuffer::append (char d) {
	int ret = spaceAvailable.wait(0);
	if (!ret) return ret;
	ret = mutexa.wait(0);
	if (!ret) {
		spaceAvailable.signal();
		return ret;
	}
	buffer[tail] = d;
	tail = (tail+1)%Size;
	mutexa.signal();
	itemAvailable.signal();
	return 0;
}

char BoundedBuffer::take () {
	while ((itemAvailable.wait(0))!=1);
	while ((mutext.wait(0))!=1);
	char d = buffer[head];
	head = (head+1)%Size;
	mutext.signal();
	spaceAvailable.signal();
	return d;
}
