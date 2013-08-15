#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Trinidad/Trinidad.h";
#include <boost/property_tree/ptree.hpp>

#include "Vladivostok/SimpleCells.h"
#include "Vladivostok/Spheres.h"

using namespace director;
using namespace TOBAGO;

int main(void) {
	//http://stackoverflow.com/questions/5700466/c-c-unix-configuration-file-library
	//boost::property_tree::ptree pTree;

	GLFWwindow *win;

	win = setup(1280, 720, 4, 2, "WdASAAAAAA", NULL);
	windows.push_back(win);
//	windows.push_back(TOBAGO::createWindow(1200, 800, "WOPYTYWO", NULL, director::windows[0]));

	Sphere aaaa(1.0, 2);

	manager = new SceneManager(&currentTime);
	Timer timer(manager);

	Camera cam(1280, 720,
			   45.0f, 1.0, 50.0,
			   glm::vec3(25.0f, 8.0f, 25.0f), glm::vec3(-1.0f, -0.5f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
			   12.5, 17.0, 4.5f);

	Rig rig(cam, -1.0);
//	Camera cam = Camera(45.0f, 1920, 1080, 2.0f, 300.0f, glm::vec3(0, 50, -150), glm::vec3(0, 0, 1), glm::vec3(0,1,0));

//	TBO thisIsIt("Images/Background.png", true);
//	director::manager->addScene(new BlurScene(&thisIsIt, 1.0, NULL), 0, 100000000, 0.1);
	Spheres yep(&rig);

	director::manager->addScene(new FrameRate(5, 5, 200, 50), 0, 1000000000, 1);
	director::manager->addScene(&yep, 0, 10000000, 0.1);

//	yep.lights->addDirectionalLight(glm::vec3(0.2, 0.1, 0.01), glm::vec3(0.0, 1.0, 1.0), glm::vec3(0.3, 0.5, 0.6));

//	yep.lights->addDirectionalLight(glm::vec3(2, 0.0, 0.0),	glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0));

//	yep.lights->addPointLight(glm::vec3(2.4, -1, 5.16), glm::vec3(2.0, 0.0, 0.5), glm::vec3(1.0, 1.0, 1.0));
//	yep.lights->addSpotLight(glm::vec3(0.4, -1, 0.16), glm::vec3(2.0, 0.01, 0.01), glm::vec3(-0.8, 0.0, 1.0), glm::vec3(1.0, 1.0, 1.0), 0.9, 0.96, 2.0);
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

//	glfwDestroyWindow(director::MainWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
