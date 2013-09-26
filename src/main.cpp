#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Trinidad/Trinidad.h";
#include <boost/property_tree/ptree.hpp>

#include "Vladivostok/Parser.h"
#include "Vladivostok/FloatingBalls.h"
#include "Vladivostok/Storm/Storm.h"
#include "Vladivostok/myGlow.h"
#include "Vladivostok/ModelTest.h"

#include "Vladivostok/Storm/Scenes/SixSeventh.h"

using namespace director;
using namespace TOBAGO;

int main(void) {
	GLFWwindow *win;

	CSParser csp("CS/Storm.cs");
	csp.parse();

	win = setup(csp.data.width, csp.data.height, csp.data.oglMajor, csp.data.oglMinor, csp.data.windowTitle.c_str(), NULL);
	windows.push_back(win);

	glfwMakeContextCurrent(windows[0]);

//	A3dsHandler test("Models/Storm/6Skull.3DS", 3);
//	test.saveNormalsToFile("Models/Storm/6SkullNormals3.txt");

	manager = new SceneManager(&currentTime);
	
	//Scenes BYPASS:
	Rig rig(csp.data.width, csp.data.height, csp.data.FOV, csp.data.zNear, csp.data.zFar, csp.data.CamPos,
		csp.data.CamDir, csp.data.CamUp, csp.data.EyeSep, csp.data.FocalLength, csp.data.FocusDistance, csp.data.FStop);

	vec3 cpos(50.0, 0.0, -135.0);

	SixthRendererSkull bypass(&csp, &rig);
	bypass.setPosition(&cpos);
	manager->addScene(&bypass, 1, 100000000000, 0.4);
	
	/*
	Storm eyes(&csp);
	myGlowScene glow(8.0, 0.4, 0, eyes.left->textures[1], eyes.right->textures[1]);
	manager->addScene(&eyes,   2, 10000000, 0.1);
	manager->addScene(&glow,   2, 10000000, 0.2);
	*/

	manager->addScene(new RenderQuad(STEREO_ANAGLYPH_RC), 4, 100000000, 1);
	//manager->addScene(new FrameRate(20, 20, 200, 80), 4, 10000000, 2);

	Timer timer(manager);

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
