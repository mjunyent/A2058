#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.

int main(void) {
	initTobago init(initTobago::useGLFW, 1280, 720, 4, 1, "Test", false);

	while(!glfwWindowShouldClose(init.glfwInit->windows[0])) {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		TOBAGO::log.write(DEBUG) << "hi handsome!";
		glfwPollEvents();
		if(glfwGetKey(init.glfwInit->windows[0], GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(init.glfwInit->windows[0], GL_TRUE);
	}

	glfwDestroyWindow(init.glfwInit->windows[0]);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
