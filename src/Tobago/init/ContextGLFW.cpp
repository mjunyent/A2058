#ifndef NO_GLFW
#include "ContextGLFW.h"

void TobagoInitGLFW(int major, int minor) {
	if(!glfwInit()) {
		Tobago.log->write(ERROR) << "Couldn't init GLFW";
		exit(EXIT_FAILURE);
	}

	glfwSetErrorCallback(glfw_error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

ContextGLFW::ContextGLFW(int width, int height, const char* name, GLFWmonitor* monitor, ContextGLFW* shared) : Context() {	
	this->width = width;
	this->height = height;
	this->windowName = name;
	this->monitor = monitor;
	this->shared = shared;
}

void ContextGLFW::init() {
	GLFWwindow* sharedWin;
	if(shared == NULL) sharedWin = NULL;
	else sharedWin = shared->window;

	window = glfwCreateWindow(width, height, windowName.c_str(), monitor, sharedWin);
	glfwSetFramebufferSizeCallback(window, glfw_framebuffersize_callback);

	if(!window) {
		Tobago.log->write(ERROR) << "GLFW could not create window " << windowName.c_str();
	}

	glfwMakeContextCurrent(window);
	enabled = true;
}

void ContextGLFW::use() {
	glfwMakeContextCurrent(window);
}

void ContextGLFW::stop() {
	if(enabled) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		glfwDestroyWindow(window);
		enabled = false;
	}
}

void ContextGLFW::swap() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void glfw_framebuffersize_callback(GLFWwindow* window, int width, int height) {
	for(Context* c : Tobago.contexts) {
		ContextGLFW* cg;
		cg = dynamic_cast<ContextGLFW*>(c);
		if(cg != NULL) {
			if(cg->window == window) {
				cg->width = width;
				cg->height = height;
				//makecontext && glViewport(0, 0, width, height);
			}
		}
	}
}

void glfw_error_callback(int error, const char* description) {
	Tobago.log->write(ERROR) << "GLFW ERROR CALLBACK: " << description;
}

#endif