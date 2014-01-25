#pragma once
#ifndef NO_GLFW
#ifndef TOBAGO_ContextGLFW
#define TOBAGO_ContextGLFW

#include "initTobago.h"
#include <GLFW/glfw3.h>
#include <vector>

void TobagoInitGLFW(int major, int minor);

class ContextGLFW : public Context {
public:
	ContextGLFW(int width, int height, const char* name, GLFWmonitor* monitor = NULL, ContextGLFW* shared = NULL);

	void init();
	void use();
	void stop();
	void swap();

	std::string windowName;
	GLFWmonitor* monitor;
	GLFWwindow* window;
	ContextGLFW* shared;
};

void glfw_error_callback(int error, const char* description);
void glfw_framebuffersize_callback(GLFWwindow* window, int width, int height);

#endif
#endif