#pragma once
#include <mutex>
#include <ctime>
#include <exception>
#define EDGLM_WIN 1
#define EDGLM_MAC 2
#define EDGLM_LINUX 3
#define EDGLM_UNIX 4
#define EDGLM_OTHER 5

#define EDGLM_LOWQUALITY 0
#define EDGLM_GRAYSCALE 0

#ifdef _WIN32
#include <Windows.h>
#elif __APPLE__ || __MACH__

#elif __linux__
#include <unistd.h>
#elif __FreeBSD__

#elif __unix || __unix__

#else

#endif

static class EDGlobal {
public:
	std::mutex EDGL_Mutex;//Mutex for multithreading - avoid parallel modification

	const static int EDGL_FLOORS = 20;

	//the detailed description can be viewed in the document

	const static int EDGL_ASCENDING_TIME = 95;	//time of ascending
	const static int EDGL_DESCENDING_TIME = 95; //time of descending 
	const static int EDGL_WAITING_TIME = 500;//time of waiting on a certain floor
	const static int EDGL_FREE_TIME = 0;
	const static int EDGL_OPEN_EXTRA_TIME = 80; 
	const static int EDGL_OPEN_EXTRA_TIME_MAX = 210;
	const static int EDGL_CLOSE_BOOST_TIME = 80; 
	const static int EDGL_REALLOCATION_TIMESLICE = 500; 
	const static int EDGL_REALLOCATION_ACCEPTANCE = 150;
	const static int EDGL_POSTPONE_TIMESLICE = 100;
	const static int EDGL_ARGV_BUFFER = 10;

	static int edglSharedTick;
	static int edglAnimationQuality;
	static int edglColorMode;
	static float edglColorMultiplier;

	static int edglGetOS();
	static int edglSleep(int milisec);
	EDGlobal();
}EDGlobals;

