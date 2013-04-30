#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Trinidad/Trinidad.h" //Trinidad contains scenes already done, just to plug & play to an sceneManager or wherever.
#include "Vidian/VesselScene.h"
#include "Vidian/OscListener.h"

int main(void) {
	int running = true;

	OGL::init(1280, 720, 0, 0, 0, 32, 64, 1, "A2058", 3, 4, GLFW_WINDOW);
	
	//OSC Listener
	GLFWmutex mutex = glfwCreateMutex();
	int status = 0;
    int command = -1;
	vector<float> costs(6, 0.3);
	GestureReceiver CatchnRun(mutex, &status, &command, &costs);
	glfwCreateThread(RunGestureReceiver, &CatchnRun);

	//Initialize some vars...
	global::song = new SoundHandler("hearthShort5.mp3", 1024);
	global::manager = new SceneManager(&global::currentTime);
	Timer timer(global::manager);

	//Initialize scenes
	Camera cam(glm::vec3(2.4f,-1, 5.16f), glm::vec3(-0.99, 0.0556, 0.123), glm::vec3(0, 1, 0), 0.05, true);
	StartDeferred Sdeferred;
	EndDeferred Edeferred(&Sdeferred);
	VesselScene vessel(Sdeferred.first, &cam.V, mutex, &status, &command);
	RenderDeferred Rdeferred(&Sdeferred, &vessel.invPV, &cam.position);
//	DebugDeferred Ddeferred(&Sdeferred, &vessel.invPV);

	//Initialize lights
	Light *lights = Rdeferred.lights;
	//Add lights! (maybe this should go inside vessel.
	lights->addDirectionalLight(glm::vec3(2, 0.0, 0.0),	-cam.direction, glm::vec3(1.0,1.0,1.0));
//	lights->addPointLight(cam.position, glm::vec3(2, 0.2, 0.05), glm::vec3(1.0, 1.0, 1.0));
//	lights->addSpotLight(cam.position, glm::vec3(2, 0.2, 0.05), cam.direction, glm::vec3(1.0, 1.0, 1.0), cos(25.0 * 3.141592 / 180.0), cos(15.0 * 3.141592 / 180.0), 2);
//	lights->addDirectionalLight(glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));

	//Add scenes to queue
	global::manager->addScene(&cam,			4,		100000000,		0);
	global::manager->addScene(&Sdeferred,	4,		100000000,		1);
	global::manager->addScene(&vessel,		4,		100000000,		4);
	global::manager->addScene(&Edeferred,	4,		100000000,		5);
	global::manager->addScene(&Rdeferred,	4,		100000000,		6);
//	global::manager->addScene(&Ddeferred,	0,		100000000,		7);
	global::manager->addScene(new SoundSpectrum, 0, 100000000,			  20000);
	global::manager->addScene(new FrameRate(5, 5, 200, 50), 0, 100000000, 20001);

	//PLAY!
	global::song->Play();

	while(running) {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		timer.update();
		global::manager->render();
		glfwSwapBuffers();
		randValue(100, 200);
		if(glfwGetKey( 'Z' )) global::song->setVel(global::song->playVel - 0.05);
		if(glfwGetKey( 'X' )) global::song->setVel(global::song->playVel + 0.05);
		//cout << global::song->playVel << endl;
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

	glfwTerminate();
	return 0;
}