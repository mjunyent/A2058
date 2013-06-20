#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Trinidad/Trinidad.h" //Trinidad contains scenes already done, just to plug & play to an sceneManager or wherever.

#include "ShaderToy/ShaderToy.h"

int main(void) {
	int running = true;
	double lastSpace = 0;
	double lastReload = 0;

	ConfRead conf("conf.txt");

	OGL::init(conf.resX, conf.resY, 0, 0, 0, 32, 64, 1, "A2058 - ShaderToy", 3, 4, GLFW_WINDOW);

	GLuint vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

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

	while(running) {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		timer.update();
		global::manager->render();
		glfwSwapBuffers();

		if(global::currentTime-lastSpace > 0.2 && glfwGetKey( GLFW_KEY_SPACE )) {
			global::song->Pause();
			lastSpace = global::currentTime;
		}
		if(global::currentTime-lastReload > conf.relTime) {
			conf.read();
			toy.reload(conf.vertex_file.c_str(), conf.fragment_file.c_str());
		}
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

	glfwTerminate();
	return 0;
}