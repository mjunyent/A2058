#include "timer.h"

Timer::Timer()
{
	this->manager = NULL;
	this->update_f = NULL;
	lastTime = 0.0f;
	cumulativeTime = 0.0f;
	FRAME_LENGHT = 1.0f/director::fps;
	MAX_ITERATIONS = 10;
#ifndef NO_SOUND
	somelier = NULL;
#endif
}

Timer::Timer(void (*update_foo)(void)) {
	this->manager = NULL;
	this->update_f = update_foo;
	lastTime = 0.0f;
	cumulativeTime = 0.0f;
	FRAME_LENGHT = 1.0f/director::fps;
	MAX_ITERATIONS = 10;
#ifndef NO_SOUND
	somelier = NULL;
#endif
}

Timer::Timer(SceneManager* manager) {
	this->update_f = NULL;
	this->manager = manager;
	lastTime = 0.0f;
	cumulativeTime = 0.0f;
	FRAME_LENGHT = 1.0f/director::fps;
	MAX_ITERATIONS = 10;
#ifndef NO_SOUND
	somelier = NULL;
#endif
}

#ifndef NO_SOUND
void Timer::getTimeFrom(SoundHandler *f) {
	somelier = f;
}
#endif

void Timer::setUpdateF(void (*update_foo)(void)) {
	this->update_f = update_foo;
}

void Timer::update() {
	int iterations = 0;

#ifndef NO_SOUND
	if(somelier == NULL) director::currentTime = glfwGetTime();
	else {
		director::currentTime = somelier->SoundTime();
	}
#endif

#ifdef NO_SOUND
	director::currentTime = glfwGetTime();
#endif

	director::dt = director::currentTime-lastTime; //1/dt = x fps
	
	cumulativeTime += director::dt; // * 0.001f;
	lastTime = director::currentTime;

	while (cumulativeTime > FRAME_LENGHT) {
		if(manager != NULL) manager->update();
		if(update_f != NULL) update_f(); // Ya funciona !
		cumulativeTime -= FRAME_LENGHT;
		iterations++;
		if(iterations > MAX_ITERATIONS) {
			TOBAGO::log.write(WARNING) << "Timer max iteration achieved";
			break;
		}
	}
}
