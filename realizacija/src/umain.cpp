// file: umain.cpp
// author: Aleksandar Abu-Samra

#include "umain.h"

extern int userMain(int, char**);

UMain::UMain(int argc, char *argv[]) : Thread("uMain") {
	this->argc = argc;
	this->argv = argv;
	
	this->start();
}

void UMain::run() {
	ret = userMain(argc, argv);
}