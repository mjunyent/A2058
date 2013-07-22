#pragma once
#ifndef TRINIDAD_TIMER
#define TRINIDAD_TIMER

#include "Tobago/Tobago.h"
#include "director.h"

//Implements a basic timer to control framerate & updates.
class Timer {
public:
	//Constructor, creates a timer without update function.
	Timer();

	//Constructor, creates a timer, the function specified will be called every FRAME_LENGHT time.
	Timer(void (*update_foo)(void));

	//Ignores the message, uses the director::manager update function.
	Timer(SceneManager* manager);

#ifndef NO_SOUND
	//Song to get time from (instead of director time).
	void getTimeFrom(SoundHandler *f);
#endif

	void setUpdateF(void (*update_foo)(void));

	//Updates the timer and calls update function if needed. This should be called every frame.
	void update();

	double fps;
	int MAX_ITERATIONS;
	double lastTime;
	double cumulativeTime;
	double FRAME_LENGHT;
	void (*update_f)(void);
	SceneManager *manager;

#ifndef NO_SOUND
	SoundHandler *somelier;
#endif
};

#endif