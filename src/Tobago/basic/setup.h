#pragma once
#ifndef TOBAGO_SETUP
#define TOBAGO_SETUP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include "log.h"
#include "../handlers/SoundHandler.h"

namespace TOBAGO {
	GLFWwindow* setup(int w, int h,
					  int major, int minor,
					  const char* name,
					  bool fullscreen);

    void initGLFW(int major, int minor); //and FMOD
	GLFWwindow* createWindow(int w, int h,
							 const char* name,
							 GLFWmonitor *monitor, GLFWwindow *window);
	void initGLEW();
	void error_callback(int error, const char* description);
}

#endif
