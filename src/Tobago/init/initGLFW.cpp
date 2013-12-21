#ifndef NO_GLFW
#include "initGLFW.h"

contextGLFW::contextGLFW(int major, int minor) {
	if( !glfwInit() ) {
		TOBAGO::log.write(ERROR) << "Couldn't init GLFW";
		exit( EXIT_FAILURE );
	}
	glfwSetErrorCallback(glfw_error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

int contextGLFW::createWindow(int width, int height, const char* name, int sharedProfile, GLFWmonitor *monitor) {
	GLFWwindow *tempWin, *sharedWin;

	if(sharedProfile == -1) sharedWin = NULL;
	else sharedWin = windows[sharedProfile];

	tempWin = glfwCreateWindow(width, height, name, monitor, sharedWin);
	
	if(!tempWin) {
		TOBAGO::log.write(ERROR) << "GLFW could not create window";
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	glfwMakeContextCurrent(tempWin);

	windows.push_back(tempWin);
	return windows.size()-1;
}

void contextGLFW::makeContextCurrent(int id) {
	glfwMakeContextCurrent(windows[id]);
}

GLFWmonitor* contextGLFW::getPrimaryMonitor() {
	return glfwGetPrimaryMonitor();
}

void glfw_error_callback(int error, const char* description) {
	TOBAGO::log.write(ERROR) << "GLFW ERROR CALLBACK: " << description;
}

#endif