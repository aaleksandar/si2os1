// file: timer.h
// author: Aleksandar Abu-Samra

#ifndef _timer_h_
#define _timer_h_

#include "thread.h"

typedef void interrupt (*TIntr)(...);

class Timer {
	static TIntr oldInterruptRoutine;
public:
	Timer();
	~Timer();
	
	static Time uptime;
	static Time stopwatch;
	
	static void interrupt newInterruptRoutine(...);
};

#endif