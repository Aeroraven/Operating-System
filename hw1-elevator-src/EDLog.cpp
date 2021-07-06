#include "EDLog.h"


void EDLog::edlgRecordLog(int iInitiator, int iLevel, std::string sInfo) {
	using namespace std;
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S -", localtime(&timep));
	std::string w = tmp;
	EDGlobals.EDGL_Mutex.lock();
	cout << w;
	switch (iLevel) {
		case edINFO:
			cout << "[INFO ]";
			break;
		case edWARNING:
			cout << "[WARN ]";
			break;
		case edFAULT:
			cout << "[ERROR]";
			break;
		default:
			cout << "[OTHER]";
			break;
	}
	cout <<std::setiosflags(ios::left)<< std::setw(15);
	switch (iInitiator) {
		case edTimerController:
			cout << "(ED_Timer)";
			break;
		case edGLUTController:
			cout << "(ED_GUI)";
			break;
		case edMain:
			cout << "(ED_Main)";
			break;
		case edDispatcher:
			cout << "(ED_Algo)";
			break;
		case edException:
			cout << "(ED_Except)";
			break;
		default:
			cout << "(ED_Unknown)";
	}
	cout << sInfo << endl;
	cout << flush;
	EDGlobals.EDGL_Mutex.unlock();
}

