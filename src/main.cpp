#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Trinidad/Trinidad.h" //Trinidad contains scenes already done, just to plug & play to an sceneManager or wherever.

int main(void) {
	int running = true;

	OGL::init(1280, 720, 0, 0, 0, 32, 64, 1, "A2058", 3, 4, GLFW_WINDOW);

	A3dsHandler vessel("Models/cell.3ds");
	vessel.makeVBO(0);
	vessel.makeNormals();

//	A3dsHandler vessel("Models/cub.3ds", 0);

//	VBO vertexs(vessel.vdata, sizeof(vessel.vdata), 0);
//	IBO indexs(vessel.fdata, sizeof(vessel.fdata));

	//cout << sizeof(vessel.vdata) << " - " << sizeof(vessel.fdata) << endl;

//	glm::perspective(fovy, aspect, znear, zfar);
	Shader renderer("Shaders/SimpleTransform.vert", "Shaders/paintNormals.frag");
	GLuint MatrixID = renderer.getUniform("MVP");

	glm::mat4 Projection = glm::perspective(75.0f, 4.0f/3.0f, 0.1f, 100.0f);
	glm::mat4 View		 = glm::lookAt( glm::vec3(-4,-4,-4),
										glm::vec3(10,10,0),
										glm::vec3(0,1,0));
	glm::mat4 Model      = glm::mat4(1.0);
	glm::mat4 MVP		 = Projection*View*Model;

	VBO sc_quad(global::quad, sizeof(global::quad), 0);
	IBO sc_quad_i(global::quad_I, sizeof(global::quad_I));

	float g_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	VBO triangle(g_vertex_buffer_data, sizeof(g_vertex_buffer_data), 0);

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

	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	while(running) {
		timer.update();
		global::manager->update();

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glEnable(GL_DEPTH_TEST);

		renderer.use();
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		vessel.vertexs->enable(3);
		vessel.normals->enable(3);
		vessel.vertexs->draw(GL_TRIANGLES);
		vessel.vertexs->disable();
		vessel.normals->disable();

/*		sc_quad.enable(3);
		sc_quad_i.draw(GL_TRIANGLES);
		sc_quad.disable();

		triangle.enable(3);
		triangle.draw(GL_TRIANGLES);
		triangle.disable();*/

//		vessel.vertexs->enable(3);
//		vessel.indexs->draw(GL_TRIANGLES);
//		vessel.vertexs->disable();

		global::manager->render();

		glfwSwapBuffers();
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

	glfwTerminate();
	return 0;
}