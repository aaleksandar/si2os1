// file: timer.cpp
// author: Aleksandar Abu-Samra

#include "timer.h"
#include <dos.h>
#include "kernel.h"
#include "thread.h"
#include "pcb.h"

TIntr Timer::oldInterruptRoutine = NULL;
Time Timer::uptime = 0;
Time Timer::stopwatch = 0;

extern void tick();

Timer::Timer () {
	Timer::oldInterruptRoutine = getvect(INTR1);
	setvect(INTR1, newInterruptRoutine);
}

Timer::~Timer() {
	setvect(INTR1, oldInterruptRoutine);
}

void interrupt Timer::newInterruptRoutine(...) {
	oldInterruptRoutine();
	tick();
	
	// wake up Timed blocks and reduce timeToWait
	Kernel::timedList->handleTimedBlock();
	
	stopwatch++;
	uptime++;
	
	if (0 != Kernel::running->timeSlice && stopwatch > Kernel::running->timeSlice) {
		Kernel::dispatch();
		stopwatch = 0;
	}
}