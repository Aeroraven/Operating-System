#pragma once
#include "FSDefs.h"
#include <vector>
#include <string>
using namespace std;
class FSCore{
public:
	static void fsMemcpy(db* src, db* dst, dd size);
	static void fsMemset(db* dst, db val, dd size);
	static void fsSplit(const string& s, vector<string>& tokens, const string& delimiters = " ");
};

