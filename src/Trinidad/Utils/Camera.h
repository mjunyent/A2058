#pragma once
#ifndef TRINIDAD_FRAMERATE
#define TRINIDAD_FRAMERATE

#include "Tobago/Tobago.h"

using namespace std;

namespace FrameRate {
	void setup(int x, int y, int width, int height);
	void draw(double time);
};

#endif