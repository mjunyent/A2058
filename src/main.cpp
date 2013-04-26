#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Trinidad/Trinidad.h" //Trinidad contains scenes already done, just to plug & play to an sceneManager or wherever.
#include "Vidian/VesselScene.h"

int main(void) {
	int running = true;

	OGL::init(1280, 720, 0, 0, 0, 32, 64, 1, "A2058", 3, 4, GLFW_WINDOW);
//	glfwSwapInterval(20);

	//Initialize some vars...
	global::manager = new SceneManager(&global::currentTime);
	Timer timer(global::manager);

	//Initialize scenes
	Camera cam(glm::vec3(2.4f,-1, 5.16f), glm::vec3(-0.99, 0.0556, 0.123), glm::vec3(0, 1, 0), 0.05, true);
	StartDeferred Sdeferred;
	EndDeferred Edeferred(&Sdeferred);
	VesselScene vessel(Sdeferred.first, &cam.V);
	RenderDeferred Rdeferred(&Sdeferred, &vessel.invPV, &cam.position);

	Light *lights = Rdeferred.lights;

	//Add lights! (maybe this should go inside vessel.
	lights->addDirectionalLight(glm::vec3(2, 0.0, 0.0),	-cam.direction, glm::vec3(1.0,1.0,1.0));
//	lights->addPointLight(cam.position, glm::vec3(2, 0.2, 0.05), glm::vec3(1.0, 1.0, 1.0));

//	lights->addSpotLight(cam.position, glm::vec3(2, 0.2, 0.05), cam.direction, glm::vec3(1.0, 1.0, 1.0), cos(25.0 * 3.141592 / 180.0), cos(15.0 * 3.141592 / 180.0), 2);
//	lights->addDirectionalLight(glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));

//	DebugDeferred Ddeferred(&Sdeferred, &vessel.invPV);

	global::manager->addScene(&cam,			0,		100000000,		0);
	global::manager->addScene(&Sdeferred,	0,		100000000,		1);
	global::manager->addScene(&vessel,		0,		100000000,		4);
	global::manager->addScene(&Edeferred,	0,		100000000,		5);
	global::manager->addScene(&Rdeferred,	0,		100000000,		6);
//	global::manager->addScene(&Ddeferred,	0,		100000000,		7);

	global::manager->addScene(new FrameRate(5, 5, 200, 50), 0, 100000000, 20001);

	while(running) {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		timer.update();
		global::manager->render();
		glfwSwapBuffers();
		randValue(100, 200);
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

	glfwTerminate();
	return 0;
}