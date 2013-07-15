#include "setup.h"

using namespace std;

void OGL::init(int w	, int h , 
	int r , int g , int b , 
	int alpha , int depth , int stencil , 
	char *name , int major , int minor ,
	int aa , GLFWmonitor *monitor ) 
{
	global::width = w;
	global::height = h;

	//////////
	// GLFW //
	//////////
	if( !glfwInit() ) {
		cerr << "glfwInit fail'd" << endl;
		exit ( EXIT_FAILURE );
	}
	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_SAMPLES, aa);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//	glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	//	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	global::MainWindow = glfwCreateWindow(w, h, name, monitor, NULL);
	if( !global::MainWindow )
	{
		global::log.error("GLFW could not create main window");
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	glfwSetInputMode(global::MainWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetWindowSizeCallback(global::MainWindow, resize_callback);
	
	glfwMakeContextCurrent(global::MainWindow);

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, w, h); //dunou if necessary.


	//////////
	// GLEW //
	//////////
	glewExperimental=GL_TRUE;
	if( glewInit()!=GLEW_OK ) {
		cerr << "glewInit fail'd" << endl;
		glfwTerminate();
		exit( EXIT_FAILURE );
	}


#ifndef NO_SOUND
	initSound();
#endif
}

void resize_callback( GLFWwindow *window, int width, int height ) {
	global::width = width;
	global::height = height;
	glViewport(0, 0, width, height); //dunou if necessary.
	cout << "UEUEUEUEUEUE" << endl;
}

void error_callback(int error, const char* description) {
	global::log.error(description);
}

/*void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

}*/