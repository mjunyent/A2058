#include "setup.h"

void TOBAGO::initGLFW(int major, int minor) 
{
	if( !glfwInit() ) {
		TOBAGO::log.write(ERROR) << "glfwInit fail'd";
		exit ( EXIT_FAILURE );
	}
	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_SAMPLES, 4);

#ifndef NO_SOUND
	initSound();
#endif
}

GLFWwindow* TOBAGO::createWindow(int w, int h, const char* name, GLFWmonitor *monitor, GLFWwindow *window) {
	GLFWwindow *ret;
	ret = glfwCreateWindow(w, h, name, monitor, window);
	if(!ret) {
		TOBAGO::log.write(ERROR) << "GLFW could not create window";
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	glfwMakeContextCurrent(ret);
	GLuint vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return ret;
}

void TOBAGO::initGLEW() {
	glewExperimental=GL_TRUE;
	GLenum err = glewInit();
	if(err != GLEW_OK) {
		TOBAGO::log.write(ERROR) << "glewInit fail'd: " << glewGetErrorString(err);
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
}

GLFWwindow* TOBAGO::setup(int w, int h, int major, int minor, const char* name, GLFWmonitor *monitor) {
	initGLFW(major, minor);
	GLFWwindow *ret;

	ret = glfwCreateWindow(w, h, name, monitor, NULL);
	if(!ret) {
		TOBAGO::log.write(ERROR) << "GLFW could not create main window";
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	glfwMakeContextCurrent(ret);
	initGLEW();

	GLuint vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	return ret;
}

void TOBAGO::error_callback(int error, const char* description) {
	TOBAGO::log.write(ERROR) << description;
}