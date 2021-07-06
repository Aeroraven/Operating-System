#include "EDGlobal.h"

EDGlobal::EDGlobal() {
	edglSharedTick = 0;
}

int EDGlobal::edglSharedTick = 0;
int EDGlobal::edglColorMode = 1;
int EDGlobal::edglAnimationQuality = 1; //Details of effects
float EDGlobal::edglColorMultiplier =1; //Global color saturation modifier

int EDGlobal::edglGetOS() {
#ifdef _WIN32
    return EDGLM_WIN;
#elif __APPLE__ || __MACH__
    return EDGLM_MAC;
#elif __linux__
    return EDGLM_LINUX;
#elif __FreeBSD__
    return EDGLM_OTHER;
#elif __unix || __unix__
    return EDGLM_UNIX;
#else
    return EDGLM_OTHER;
#endif
}

int EDGlobal::edglSleep(int milisec) {
#ifdef _WIN32
    Sleep(milisec);
#elif __APPLE__ || __MACH__
    clock_t start = clock();
    while (clock() - start <= milsec) {
    }
#elif __linux__
    usleep(milisec * 1000);
#else
    clock_t start = clock();
    while (clock() - start <= milsec) {
    }
#endif
    return 0;
}