#pragma once
#ifndef TRINIDAD_DIRECTOR
#define TRINIDAD_DIRECTOR

#include "Tobago/Tobago.h"
#include "SceneManager.h"

namespace director {
	extern std::vector<GLFWwindow*> windows;
	extern int						currentWindow;

	extern double					fps;				//Desired framerate.
	extern double					currentTime;
	extern double					dt;					//Frame increment.

	extern float					quad[12];			//Screen Quad.
	extern GLushort					quad_I[6];

	extern SceneManager				*manager;

#ifndef NO_SOUND
	extern SoundHandler				*song;
#endif

};

#endif
