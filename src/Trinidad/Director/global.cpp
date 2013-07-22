#include "global.h"

namespace DIRECTOR {
	std::vector<GLFWwindow*> windows;
	int currentWindow = 0;

	float quad[] = { 
		-1.0f,  1.0f, 0.0f,	//0 UP, LEFT
		1.0f,  1.0f, 0.0f, //1 UP, RIGHT
		1.0f, -1.0f, 0.0f, //2 DOWN, RIGHT
		-1.0f, -1.0f, 0.0f  //3 DOWN, LEFT
	};

	GLushort quad_I[] = {
		0, 3, 1,
		1, 3, 2
	};

	double  fps   = 60;
	double  currentTime = 0;
	double  dt = 0;

	SceneManager *manager;

#ifndef NO_SOUND
	SoundHandler *song;
#endif

};
