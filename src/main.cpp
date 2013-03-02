#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Trinidad/Trinidad.h" //Trinidad contains scenes already done, just to plug & play to an sceneManager or wherever.

int main(void) {
	int running = true;

	OGL::init(1280, 720, 0, 0, 0, 32, 64, 1, "A2058", 3, 4, GLFW_WINDOW);

	glm::mat4 Projection = glm::perspective(75.0f, 4.0f/3.0f, 0.1f, 100.0f);
	glm::mat4 View		 = glm::lookAt( glm::vec3(-4,-4,-4),
										glm::vec3(10,10,0),
										glm::vec3(0,1,0));
	glm::mat4 Model      = glm::mat4(1.0);
	glm::mat4 MVP		 = Projection*View*Model;
	float angle = 0.0;

	//Initialize some vars...
	global::manager = new SceneManager(&global::currentTime);
	global::song = new SoundHandler("demo.mp3", 1024);
	Timer timer(global::manager);

	//Configure some things...
	timer.getTimeFrom(global::song);

	//Initialize scenes
	Camera cam(glm::vec3(4,4,4), glm::vec3(-1, -1, -1), glm::vec3(0, 1, 0), 0.5, true, &Projection);
	global::manager->addScene(&cam, 0, 10000000, 0);
	global::manager->addScene(new RenderModel("Shaders/SimpleTransform.vert", "Shaders/paintNormals.frag", "Models/cell.3ds", 0, &MVP),
								0,
								10000000,
								1);
	global::manager->addScene(new FrameRate(5, 5, 200, 50), 0, 100000000, 20001);
	global::manager->addScene(new SoundSpectrum(),			0, 100000000, 20000);

	//PLAY!
	global::song->Play();

	while(running) {
		timer.update();

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		View = cam.V;
		Model = glm::rotate(angle, glm::vec3(1, 0, 0));
//		angle += 0.5;

		MVP = Projection*View*Model;

		global::manager->render();

		glfwSwapBuffers();
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

	glfwTerminate();
	return 0;
}