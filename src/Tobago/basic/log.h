#pragma once
#ifndef TOBAGO_LOG
#define TOBAGO_LOG

#include <cstring>
#include <iostream>
#include <fstream>
#include <GLFW/glfw3.h>

using namespace std;
//http://stackoverflow.com/questions/5028302/small-logger-class

enum LogLevel { ERROR, WARNING, INFO, DEBUG };

class Log {
public:
    Log(const char* name);

	virtual ~Log();

	ofstream& write(LogLevel ERRNO);

private:
	ofstream *m_out; //ostream to be more flexible.
};

namespace TOBAGO {
	extern Log log;
}

#endif
