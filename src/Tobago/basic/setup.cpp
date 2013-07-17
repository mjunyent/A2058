#include "setup.h"

using namespace std;

void OGL::init(int w	, int h , 
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

	int count,t;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	cout << count << endl;
	cin >> t;

	const GLFWvidmode* mode = glfwGetVideoMode(monitors[t]);
	
	cout << mode->width << ", " << mode->height << " - " << mode->refreshRate << endl;
	w = mode->width;
	h = mode->height;

	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_SAMPLES, aa);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//	glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	//	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	global::MainWindow = glfwCreateWindow(w, h, name, monitors[t], NULL);
	if( !global::MainWindow )
	{
		global::log.error("GLFW could not create main window");
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	glfwSetInputMode(global::MainWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetWindowSizeCallback(global::MainWindow, resize_callback);
	glfwMakeContextCurrent(global::MainWindow);

	glfwSwapInterval(0);
	//////////
	// GLEW //
	//////////
	glewExperimental=GL_TRUE;
	if( glewInit()!=GLEW_OK ) {
		cerr << "glewInit fail'd" << endl;
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, w, h); //dunou if necessary.

	GLuint vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

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