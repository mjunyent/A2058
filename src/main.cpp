#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.

#include <boost/property_tree/ptree.hpp>

int main(void) {
	//http://stackoverflow.com/questions/5700466/c-c-unix-configuration-file-library
	//boost::property_tree::ptree pTree;

	GLFWwindow *win;

	win = TOBAGO::setup(1200, 800, 3, 2, "WASAAAAAA", NULL);

	while(!glfwWindowShouldClose(win)) {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		/*
		timer.update();
		global::manager->render();
		glfwSwapBuffers(global::MainWindow);
		glfwPollEvents();

		if(global::currentTime-lastSpace > 0.2 && glfwGetKey(global::MainWindow, GLFW_KEY_SPACE )) {
			global::song->Pause();
			lastSpace = global::currentTime;
		}
		if(global::currentTime-lastReload > conf.relTime) {
			conf.read();
			toy.reload(conf.vertex_file.c_str(), conf.fragment_file.c_str());
		}*/
		glfwSwapBuffers(win);
		glfwPollEvents();
		if(glfwGetKey(win, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(win, GL_TRUE);
	}

//	glfwDestroyWindow(global::MainWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}