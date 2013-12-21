#pragma once
#ifndef NO_GLFW
#ifndef TOBAGO_INITGLFW
#define TOBAGO_INITGLFW

#include <GLFW/glfw3.h>
#include <vector>

#include "../utility/log.h"

class contextGLFW {
public:
	contextGLFW(int major, int minor);

	int createWindow(int width, int height, const char* name, int sharedProfile = -1, GLFWmonitor *monitor = NULL);
	void makeContextCurrent(int id);
	GLFWmonitor* getPrimaryMonitor();

	vector<GLFWwindow*> windows;
};

void glfw_error_callback(int error, const char* description);

#endif
#endif