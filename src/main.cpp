#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Trinidad/Trinidad.h" //Trinidad contains scenes already done, just to plug & play to an sceneManager or wherever.

#include "ShaderToy/ShaderToy.h"

#include <boost/property_tree/ptree.hpp>

int main(void) {
	double lastSpace = 0;
	double lastReload = 0;
	//http://stackoverflow.com/questions/5700466/c-c-unix-configuration-file-library
	boost::property_tree::ptree pTree;

	ConfRead conf("conf.txt");

	OGL::init(conf.resX, conf.resY, "A2058 - ShaderToy", 3, 2, 4, NULL);

	//Initialize some vars...
	global::song = new SoundHandler(conf.song_file.c_str(), 1024);
	global::manager = new SceneManager(&global::currentTime);
	Timer timer(global::manager);
	ShaderToy toy(conf.vertex_file.c_str(), conf.fragment_file.c_str());


	//Add scenes to queue
	global::manager->addScene(new FrameRate(5, 5, 200, 50), 0,		100000000,  20001);
	global::manager->addScene(&toy, 0, 10E12, 1);

	//PLAY!
	global::song->Play();

	while(!glfwWindowShouldClose(global::MainWindow)) {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		timer.update();
		global::manager->render();
		glfwSwapBuffers(global::MainWindow);
		glfwPollEvents();

		if(global::currentTime-lastSpace > 0.2 && glfwGetKey(global::MainWindow, GLFW_KEY_SPACE )) {
			global::song->Pause();
			lastSpace = global::currentTime;
		}
		if(global::currentTime-lastReload > conf.relTime) {
			conf.read();
			toy.reload(conf.vertex_file.c_str(), conf.fragment_file.c_str());
		}
		if(glfwGetKey(global::MainWindow, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(global::MainWindow, GL_TRUE);
	}

	glfwDestroyWindow(global::MainWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}