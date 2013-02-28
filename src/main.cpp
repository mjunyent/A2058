#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Trinidad/Trinidad.h" //Trinidad contains scenes already done, just to plug & play to an sceneManager or wherever.

int main(void) {
	int running = true;

	OGL::init(1280, 720, 0, 0, 0, 32, 64, 1, "A2058", 3, 4, GLFW_WINDOW);

	A3dsHandler vessel("Models/vessel.3ds", 1);

//	glm::perspective(fovy, aspect, znear, zfar);


	//Initialize some vars...
	global::manager = new SceneManager(&global::currentTime);
	global::song = new SoundHandler("demo.mp3", 1024);
	Timer timer;

	//Configure some things...
	timer.getTimeFrom(global::song);

	//Initialize scenes
	SoundSpectrum::setup();
	FrameRate::setup(5, 5, 200, 50);

	//Add them to timeline
	global::manager->addEvent(0, 10000000, 2001, NULL, NULL, &SoundSpectrum::draw, &SoundSpectrum::update);
	global::manager->addEvent(0, 10000000, 2000, NULL, NULL, &FrameRate::draw, NULL);

	//PLAY!
	global::song->Play();

	while(running) {
		timer.update();
		global::manager->update();

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		vessel.vertexs->enable(3);
		vessel.indexs->draw(GL_TRIANGLES);
		vessel.vertexs->disable();

		global::manager->render();

		glfwSwapBuffers();
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

	glfwTerminate();
	return 0;
}