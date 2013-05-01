#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Trinidad/Trinidad.h" //Trinidad contains scenes already done, just to plug & play to an sceneManager or wherever.
#include "Vidian/VesselScene.h"
#include "Vidian/OscListener.h"
#include "Vidian/ConfRead.h"

int main(void) {
	int running = true;

	ConfRead Config("conf.txt");

	OGL::init(Config.ResX, Config.ResY, 0, 0, 0, 32, 64, 1, "A2058", 3, 4, Config.WindowMode);

	//OSC Listener
	GLFWmutex mutex = glfwCreateMutex();
	int status = 0;
    int command = -1;
	GestureReceiver CatchnRun(mutex, &status, &command, &Config.costs);
	glfwCreateThread(RunGestureReceiver, &CatchnRun);

	//Initialize some vars...
	global::song = new SoundHandler("hearthShort5.mp3", 1024);
	global::manager = new SceneManager(&global::currentTime);
	Timer timer(global::manager);

	//Initialize RenderBuffers.
	bool qalite[1] = { true };
	FBO PreLeftBuffer(global::width, global::height, true, 1, qalite);
	FBO PreRightBuffer(global::width, global::height, true, 1, qalite);
	FBO LeftBuffer(global::width, global::height, true, 1, qalite);
	FBO RightBuffer(global::width, global::height, true, 1, qalite);

	//Initialize scenes
	Rig rig(&Config.position, &Config.direction, &Config.up, &Config.EyeSep, &Config.CenterDist, &LeftBuffer, &RightBuffer, Config.mode3d, true); 

	StartDeferred RightSdeferred;
	StartDeferred LeftSdeferred;

	EndDeferred RightEdeferred(&RightSdeferred);
	EndDeferred LeftEdeferred(&LeftSdeferred);

	VesselScene vessel(RightSdeferred.first, mutex, &status, &command);

	VesselRender right(&vessel, &rig.V_Right);
	VesselRender left(&vessel, &rig.V_Left);
	
	RenderDeferred RightRdeferred(&RightSdeferred, &right.invPV, &rig.p_right);
	RenderDeferred LeftRdeferred(&LeftSdeferred, &left.invPV, &rig.p_left);



	//Initialize lights
	Light *lights = RightRdeferred.lights;
	LeftRdeferred.lights = lights; //TOOOOOOMA GUARRADA
	//Add lights! (maybe this should go inside vessel.
	lights->addDirectionalLight(glm::vec3(2, 0.0, 0.0),	-Config.direction, glm::vec3(1.0,1.0,1.0));
//	lights->addPointLight(cam.position, glm::vec3(2, 0.2, 0.05), glm::vec3(1.0, 1.0, 1.0));
//	lights->addSpotLight(cam.position, glm::vec3(2, 0.2, 0.05), cam.direction, glm::vec3(1.0, 1.0, 1.0), cos(25.0 * 3.141592 / 180.0), cos(15.0 * 3.141592 / 180.0), 2);
//	lights->addDirectionalLight(glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));

	//Add scenes to queue
	//Basic Setup:
	global::manager->addScene(&vessel,						4,		100000000,		1);
	
	//Render Right
	global::manager->addScene(&RightSdeferred,				4,		100000000,		2);
	global::manager->addScene(&right,						4,		100000000,	    3);
	global::manager->addScene(&RightEdeferred,				4,		100000000,		4);
	global::manager->addScene(new BindFBO(&PreRightBuffer),	4,		100000000,		5);
	global::manager->addScene(&RightRdeferred,				4,		100000000,		6);
	global::manager->addScene(new UnbindFBO(&PreRightBuffer),	4,		100000000,		7);

	global::manager->addScene(new InfoRender(&vessel, &rig.V_Right, &PreRightBuffer, &RightBuffer), 4, 1000000, 7.5);

	//Render Left
	global::manager->addScene(&LeftSdeferred,				4,		100000000,		8);
	global::manager->addScene(&left,						4,		100000000,	    9);
	global::manager->addScene(&LeftEdeferred,				4,		100000000,	   10);
	global::manager->addScene(new BindFBO(&PreLeftBuffer),		4,		100000000,	   11);
	global::manager->addScene(&LeftRdeferred,				4,		100000000,	   12);
	global::manager->addScene(new UnbindFBO(&PreLeftBuffer),	4,		100000000,	   13);
	
	global::manager->addScene(new InfoRender(&vessel, &rig.V_Left, &PreLeftBuffer, &LeftBuffer), 4, 10000000, 13.5);

	//Render final image
	global::manager->addScene(&rig,							4,		100000000,	   14);

	global::manager->addScene(new FrameRate(5, 5, 200, 50), 0,		100000000,  20001);

	//PLAY!
	global::song->Play();

	while(running) {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		timer.update();
		global::manager->render();
		glfwSwapBuffers();

//		if(glfwGetKey( 'Z' )) global::song->setVel(global::song->playVel - 0.05);
//		if(glfwGetKey( 'X' )) global::song->setVel(global::song->playVel + 0.05);

		if(glfwGetKey( GLFW_KEY_SPACE ) || Config.MODE == "DEBUG") Config.read();

		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

	glfwTerminate();
	return 0;
}