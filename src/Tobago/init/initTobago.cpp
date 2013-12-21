#include "initTobago.h"

initTobago::initTobago(contextProvider context,
					   int width, int height,
					   int major, int minor,
					   const char* name, bool fullscreen) {
	//OpenGL context and Window
	#ifndef NO_GLFW
	if(context == useGLFW) {
		glfwInit = new contextGLFW(major, minor);
		GLFWmonitor *monitor;
		if(fullscreen) monitor = glfwInit->getPrimaryMonitor();
		else monitor = NULL;

		glfwInit->createWindow(width, height, name, -1, monitor);
	}
	#endif

	//GLEW
	glewExperimental=GL_TRUE;
	GLenum err = glewInit();
	if(err != GLEW_OK) {
		TOBAGO::log.write(ERROR) << "glewInit fail'd: " << glewGetErrorString(err);
		exit( EXIT_FAILURE );
	}

	//FMOD
	#ifndef NO_FMOD
	initSound();
	#endif

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	this->openGLMajor = major;
	this->openGLMinor = minor;
	this->width = width;
	this->height = height;
}