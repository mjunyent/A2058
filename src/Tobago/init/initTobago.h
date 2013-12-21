#pragma once
#ifndef TOBAGO_INITTOBAGO
#define TOBAGO_INITTOBAGO

#include <GL/glew.h>
#ifdef GLEWLIB
	#pragma comment(lib, "glew32s.lib");
#endif

#include "initGLFW.h"

#include <glm/glm.hpp> //GLM Libs...
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "../handlers/SoundHandler.h"
#include "../utility/log.h"

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

//Initializes:
// an OpenGL context
// GLEW
// FMOD (if enabled)
// a window
class initTobago {
public:
	enum contextProvider {
		#ifndef NO_GLFW
			useGLFW
		#endif
	};

	initTobago(contextProvider context,
			   int width, int height,
			   int major, int minor,
			   const char* name, bool fullscreen);

	int openGLMajor, openGLMinor, width, height;

	#ifndef NO_GLFW
		contextGLFW *glfwInit;
	#endif
};

#endif
