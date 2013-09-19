#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Trinidad/Trinidad.h";
#include <boost/property_tree/ptree.hpp>

#include "Vladivostok/Parser.h"
#include "Vladivostok/FloatingBalls.h"
#include "Vladivostok/Storm/Storm.h"
#include "Vladivostok/myGlow.h"
#include "Vladivostok/ModelTest.h"

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
	myGlowScene glow(8.0, 0.4, 0, eyes.left->textures[1], eyes.right->textures[1]);

	manager->addScene(&eyes,   4, 10000000, 0.1);
	manager->addScene(&glow,   4, 10000000, 0.2);

/*	Rig *rig = new Rig(csp.data.width, csp.data.height,
					   45.0, 0.1, 2.0,
					   glm::vec3(0, 0, -1), glm::vec3(0, 0, 1), glm::vec3(0,1,0),
					   0.05, 1, 1, 1);						

	manager->addScene(new ModelTest(rig), 0, 1000000, 0.1);*/

	manager->addScene(new RenderQuad(STEREO_ANAGLYPH_RC), 4, 100000000, 1);
//	manager->addScene(new FrameRate(20, 20, 200, 80), 4, 10000000, 2);

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
//25.084