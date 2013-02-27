#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "lib3ds.h"

//#include "Trinidad/Utils/SoundSpectrum.h"

int main(void) {
	int running = true;

	Lib3dsFile *f = 0;
	f = lib3ds_file_new();

//	Lib3dsMesh *mesh;

	OGL::init(1280, 720, 0, 0, 0, 32, 64, 1, "A2058", 3, 4, GLFW_WINDOW);

	//Initialize some vars...
	global::manager = new SceneManager(&global::currentTime);
	global::song = new SoundHandler("demo.mp3", 1024);
	Timer timer;

	//Configure some things...
	timer.getTimeFrom(global::song);

	SoundSpectrum::setup();
	FrameRate::setup(5, 5, 200, 50);

	global::manager->addEvent(0, 10000000, 2001, NULL, NULL, &SoundSpectrum::draw, &SoundSpectrum::update);
	global::manager->addEvent(0, 10000000, 2000, NULL, NULL, &FrameRate::draw, NULL);

	//PLAY!
	global::song->Play();

	while(running) {
		timer.update();
		global::manager->update();

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


		global::manager->render();

		glfwSwapBuffers();
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

	glfwTerminate();
	return 0;
}