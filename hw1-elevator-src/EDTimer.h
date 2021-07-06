#pragma once
#include<thread>
#include<vector>
#include<stdarg.h>
#include<ctime>
#include<sstream>
#include <iostream>
#include <chrono>
#include"EDLog.h"

class EDTimer{
public:
	//Timer thread
	std::thread* pTimerThread = nullptr;
	bool bExecuteOnce = false;
	//When receive the signal, the timer stops by exiting the loop
	bool bTerminationSignal = false;
	//Timer interval
	int iInterval = 1000;
	//Function pointer
	void (*pExecuteHandler)(int, const std::vector<void*>&) = nullptr;

	//arguments
	std::vector<void*>  vArgs;
	int iArgCnt;
	//block the thread (sleep)
	static void edtDelay(int);
	//thread main procedure
	static void edtThreadMain(EDTimer* self, int iArgCnt, const std::vector<void*>& vArgList);
	void edtSetup(void (*pHandler)(int, const std::vector<void*>&),int iArgCnt, std::vector<void*> vArgs, int iIntv = 1000, bool bExecOnce = false);
};

class EDTimerController {
public:
	static std::vector<EDTimer*>* edtcTimerList;
	static void edtcAddTimer(EDTimer*);
	static void edtcDemolishAll();
	static void edtcThreadJoin();

	EDTimerController();
};