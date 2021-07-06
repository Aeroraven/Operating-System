#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctime>
#include"EDGlobal.h"
#include <mutex>
#include <iomanip>
static class EDLog {
public:
	enum edlgLevels
	{
		edINFO = 0,
		edWARNING = 1,
		edFAULT = 2
	};
	enum edlgModules
	{
		edTimerController = 1,
		edGLUTController = 2,
		edMain = 3,
		edDispatcher = 4,
		edException = 5,
	};
	static void edlgRecordLog(int, int, std::string);
}EDLogClass;
