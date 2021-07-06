#include "EDTimer.h"
void EDTimer::edtThreadMain(EDTimer* self, int iArgCnt, const std::vector<void*>& vArgList) {
	while (1) {
		edtDelay(self->iInterval);
		if (self->pExecuteHandler != nullptr) {
			self->pExecuteHandler(iArgCnt, vArgList);
		}

		if (self->bExecuteOnce || self->bTerminationSignal) {
			return;
		}
	}
	std::stringstream logs;
	logs << "Timer Deactivated - " << std::hex << " ThreadPtr:" << self->pTimerThread;
	EDLogClass.edlgRecordLog(EDLogClass.edTimerController, EDLogClass.edINFO, logs.str());
}
void EDTimer::edtDelay(int milsec) {
	std::chrono::milliseconds slp(milsec);
	std::this_thread::sleep_for(slp);
}
void EDTimer::edtSetup(void (*pHandler)(int, const std::vector<void*>&), int iArgCnts, std::vector<void*> vArg, int iIntv, bool bExecOnce) {
	iArgCnt = iArgCnts;
	vArgs = vArg;
	pExecuteHandler = pHandler;
	iInterval = iIntv;
	pTimerThread = new std::thread(edtThreadMain, this, iArgCnt, vArgs);
	//Log Generate
	std::stringstream logs;
	logs << "Timer Created - FuncPtr:" << std::hex << (long long)(pHandler) << " Interval:" << std::dec << iIntv << std::hex << " ThreadPtr:" << (long long)pTimerThread;
	logs << std::dec;
	EDLogClass.edlgRecordLog(EDLogClass.edTimerController, EDLogClass.edINFO, logs.str());
}



void EDTimerController::edtcAddTimer(EDTimer* p) {
	edtcTimerList->push_back(p);
}
void EDTimerController::edtcDemolishAll() {
	for (int i = 0; i < edtcTimerList->size(); i++) {
		(*edtcTimerList)[i]->bTerminationSignal = true;
		std::stringstream logs;
		logs << "Sending Timer Deactivation Signal - " << std::hex << " ThreadPtr:" << (long long)(*edtcTimerList)[i]->pTimerThread;
		EDLogClass.edlgRecordLog(EDLogClass.edTimerController, EDLogClass.edINFO, logs.str());
	}
}

void EDTimerController::edtcThreadJoin() {
	EDLogClass.edlgRecordLog(EDLogClass.edTimerController, EDLogClass.edINFO, "Sending Timer Deactivation Signals");
	edtcDemolishAll();
	EDLogClass.edlgRecordLog(EDLogClass.edTimerController, EDLogClass.edINFO, "Waiting Timer Threads Join");
	for (int i = 0; i < edtcTimerList->size(); i++) {
		(*edtcTimerList)[i]->pTimerThread->join();
	}
	EDLogClass.edlgRecordLog(EDLogClass.edTimerController, EDLogClass.edINFO, "Timers Unloaded Successfully");

}

std::vector<EDTimer*>* EDTimerController::edtcTimerList = nullptr;

EDTimerController::EDTimerController() {
	if (edtcTimerList == nullptr) {
		edtcTimerList = new std::vector<EDTimer*>();
	}
}