// file: main.cpp
// author: Aleksandar Abu-Samra

#include <iostream.h>
#include "kernel.h"

long uptime = 0;

int main(int argc, char *argv[]) {	
	int ret = Kernel::begin(argc, argv);
	Kernel::end();
	
	cout << "..." << endl;
	cout << "uptime: " << uptime * 55 / 1000 << " seconds" << endl;
	cout << "exit code: " << ret << endl;
	
	return ret;
}