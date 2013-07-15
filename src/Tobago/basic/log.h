#pragma once
#ifndef log_h
#define log_h

#include <cstring>
#include <iostream>
#include <fstream>

#include "GLFW/glfw3.h"

using namespace std;

class Log {
public:
    Log(char* name);
    void error(const char* message);
    void warning(const char* message);
    void message(const char* message);
    void close();

private:
    char* filename;
    ofstream *output;
};

#endif
