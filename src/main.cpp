#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Trinidad/Trinidad.h";
#include <boost/property_tree/ptree.hpp>

#include "Vladivostok/SimpleCells.h"
#include "Vladivostok/Spheres.h"

using namespace director;
using namespace TOBAGO;

int main(void) {
	GLFWwindow *win;

	win = setup(1280, 720, 4, 2, "WdASAAAAAA", NULL);
	windows.push_back(win);

	manager = new SceneManager(&currentTime);
	Timer timer(manager);

	Camera cam(1280, 720,
			   45.0f, 2.0, 160.0,
			   glm::vec3(-25.0f, 18.0f, -25.0f), glm::vec3(1.0f, -0.5f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
			   14.0, 28.1f, 2.5f);

	Rig rig(cam, 0.2);

	Spheres yep(&cam);
	yep.renderOfscreen();

//	director::manager->addScene(new FrameRate(5, 5, 200, 50), 0, 1000000000, 1);
	director::manager->addScene(&yep, 0, 10000000, 0.1);
	director::manager->addScene(new BlurScene(5, 0.0), 0, 10000000000, 0.2);

	while(!glfwWindowShouldClose(win)) {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		timer.update();
		director::manager->render();

	//	glfwSwapBuffers(win);
		glfwPollEvents();
		if(glfwGetKey(win, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(win, GL_TRUE);
	}

	for(int i=0; i<windows.size(); i++)
		glfwDestroyWindow(director::windows[i]);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
