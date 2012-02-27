// file: umain.h
// author: Aleksandar Abu-Samra

#ifndef _umain_h_
#define _umain_h_

#include "thread.h"

class UMain: public Thread {
	int argc;
	char **argv;
	
public:
	int ret;
	
	UMain(int, char **);
	
	void run();
};

#endif