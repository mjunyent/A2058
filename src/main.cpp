#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Trinidad/Trinidad.h";
#include <boost/property_tree/ptree.hpp>

using namespace director;
using namespace TOBAGO;

int main(void) {
	//http://stackoverflow.com/questions/5700466/c-c-unix-configuration-file-library
	//boost::property_tree::ptree pTree;

	GLFWwindow *win;

	win = setup(1200, 800, 3, 2, "WASAAAAAA", NULL);
	windows.push_back(win);

	windows.push_back(TOBAGO::createWindow(1200, 800, "WOPYTYWO", NULL, director::windows[0]));

	manager = new SceneManager(&currentTime);
	Timer timer(manager);

	director::manager->addScene(new FrameRate(5, 5, 200, 50), 0, 1000000000, 1);


	while(!glfwWindowShouldClose(win)) {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		timer.update();
		director::manager->render();

	//	glfwSwapBuffers(win);
		glfwPollEvents();
		if(glfwGetKey(win, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(win, GL_TRUE);
	}

//	glfwDestroyWindow(director::MainWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
