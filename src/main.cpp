#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Trinidad/Trinidad.h";
#include <boost/property_tree/ptree.hpp>

#include "Vladivostok/Parser.h"
#include "Vladivostok/FloatingBalls.h"
#include "Vladivostok/Storm.h"

using namespace director;
using namespace TOBAGO;

int main(void) {
	GLFWwindow *win;

	CSParser csp("CS/Storm.cs");
	csp.parse();

	win = setup(csp.data.width, csp.data.height, csp.data.oglMajor, csp.data.oglMinor, csp.data.windowTitle.c_str(), NULL);
	windows.push_back(win);

	manager = new SceneManager(&currentTime);
	Timer timer(manager);
	glfwMakeContextCurrent(windows[0]);

	Storm eyes(&csp);
//	manager->addScene(new FrameRate(5, 5, 200, 50), 0, 1000000000, 1);
	manager->addScene(&eyes,   0, 10000000, 0.1);
	manager->addScene(new DOFScene(eyes.myCam), 0, 1000000000, 0.2);
	director::manager->addScene(new RenderQuad(STEREO_ANAGLYPH_RC), 0, 100000000, 1);

	while(!glfwWindowShouldClose(win)) {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		timer.update();
		manager->render();
		glfwPollEvents();
		if(glfwGetKey(win, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(win, GL_TRUE);
	}

	for(int i=0; i<windows.size(); i++)	glfwDestroyWindow(director::windows[i]);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
