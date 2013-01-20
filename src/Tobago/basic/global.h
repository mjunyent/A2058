#pragma once
#ifndef Base_global_h
#define Base_global_h

#include "timer.h"
#include "log.h"

namespace global {
	extern int      width;
    extern int      height;

	//Desired framerate
	extern unsigned char      fps;
	//Saves the demo current time.
	extern double   currentTime;
	//Frame increment.
	extern double	dt;

	extern float quad[12];
	extern GLushort quad_I[6];

	//If debugging, log file.
	extern Log      log;
};

#endif
