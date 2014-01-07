#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Trinidad/Trinidad.h";
#include <boost/property_tree/ptree.hpp>

#include "Vladivostok/Parser.h"
#include "Vladivostok/FloatingBalls.h"
#include "Vladivostok/Storm/Storm.h"
#include "Vladivostok/myGlow.h"
#include "Vladivostok/ModelTest.h"

#include "Vladivostok/Storm/Scenes/SixSeventh.h"
//#define SCENE
using namespace director;
using namespace TOBAGO;

int _CrtSetReportMode( 
   int reportType,
   int reportMode 
);

int main(void) {
	GLFWwindow *win;

	CSParser csp("CS/Storm.cs");
	csp.parse();

//	win = setup(1920, 1080, csp.data.oglMajor, csp.data.oglMinor, csp.data.windowTitle.c_str(), true);
	if(csp.data.fullscreen) win = setup(3840, 720, csp.data.oglMajor, csp.data.oglMinor, csp.data.windowTitle.c_str(), true);
	else win = setup(csp.data.width, csp.data.height, csp.data.oglMajor, csp.data.oglMinor, csp.data.windowTitle.c_str(), NULL);

	windows.push_back(win);

	glfwMakeContextCurrent(windows[0]);

	if(csp.data.fullscreen)	glViewport(1280, 0, 2560, 720);

//	A3dsHandler test("Models/Storm/2PlacodermOUT.3DS", 0);
//	test.saveNormalsToFile("Models/Storm/2PlacodermOUTNormals.txt");

	manager = new SceneManager(&currentTime);
	Storm eyes(&csp);
	myGlowScene glow(8.0, 0.4, 0, eyes.left->textures[1], eyes.right->textures[1]);


	TBO LoadingImage("Images/Loading.fw.png", true);
	Shader LoadingShader("Shaders/Post/general.vert", "Shaders/Vladivostok/loading.frag");
	GLint texId = LoadingShader.getUniform("Tex");
	GLint tillId = LoadingShader.getUniform("till");
	VBO VQuad(director::quad, sizeof(quad), 0);
	IBO IQuad(director::quad_I, sizeof(quad_I));

	eyes.c->selectedCell = 1;

	eyes.s->load(0);
	eyes.s->load(1);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	LoadingShader.use();
	LoadingImage.bind(0);
	glUniform1i(texId, 0);
	glUniform1f(tillId, 300.0/2560.0);
	VQuad.enable(3);
	IQuad.draw(GL_TRIANGLES);
	VQuad.disable();
	glfwSwapBuffers(win);
	
	eyes.s->load(2);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	LoadingShader.use();
	LoadingImage.bind(0);
	glUniform1i(texId, 0);
	glUniform1f(tillId, 600.0/2560.0);
	VQuad.enable(3);
	IQuad.draw(GL_TRIANGLES);
	VQuad.disable();
	glfwSwapBuffers(win);

	eyes.s->load(3);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	LoadingShader.use();
	LoadingImage.bind(0);
	glUniform1i(texId, 0);
	glUniform1f(tillId, 690.0/2560.0);
	VQuad.enable(3);
	IQuad.draw(GL_TRIANGLES);
	VQuad.disable();
	glfwSwapBuffers(win);

	eyes.s->load(4);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	LoadingShader.use();
	LoadingImage.bind(0);
	glUniform1i(texId, 0);
	glUniform1f(tillId, 1250.0/2560.0);
	VQuad.enable(3);
	IQuad.draw(GL_TRIANGLES);
	VQuad.disable();
	glfwSwapBuffers(win);
	
	eyes.s->load(5);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	LoadingShader.use();
	LoadingImage.bind(0);
	glUniform1i(texId, 0);
	glUniform1f(tillId, 1550.0/2560.0);
	VQuad.enable(3);
	IQuad.draw(GL_TRIANGLES);
	VQuad.disable();
	glfwSwapBuffers(win);

	eyes.s->load(6);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	LoadingShader.use();
	LoadingImage.bind(0);
	glUniform1i(texId, 0);
	glUniform1f(tillId, 1870.0/2560.0);
	VQuad.enable(3);
	IQuad.draw(GL_TRIANGLES);
	VQuad.disable();
	glfwSwapBuffers(win);

	eyes.s->load(7);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	LoadingShader.use();
	LoadingImage.bind(0);
	glUniform1i(texId, 0);
	glUniform1f(tillId, 2220.0/2560.0);
	VQuad.enable(3);
	IQuad.draw(GL_TRIANGLES);
	VQuad.disable();
	glfwSwapBuffers(win);

	eyes.s->load(8);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	LoadingShader.use();
	LoadingImage.bind(0);
	glUniform1i(texId, 0);
	glUniform1f(tillId, 2560.0/2560.0);
	VQuad.enable(3);
	IQuad.draw(GL_TRIANGLES);
	VQuad.disable();
	glfwSwapBuffers(win);

//	manager->addScene(new FrameRate(20, 20, 200, 80), 4, 10000000, 2);

	bool close = false;
	while(!close) {
		if(glfwGetKey(win, GLFW_KEY_SPACE)) close = true;
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		LoadingShader.use();
		LoadingImage.bind(0);
		glUniform1i(texId, 0);
		glUniform1f(tillId, 2560.0/2560.0);
		VQuad.enable(3);
		IQuad.draw(GL_TRIANGLES);
		VQuad.disable();
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	Timer timer(manager);

	manager->addScene(&eyes, director::currentTime+4, 10000000, 0.1);
	manager->addScene(&glow, director::currentTime+4, 10000000, 0.2);
	manager->addScene(new RenderQuad(STEREO_NONE), director::currentTime+4, 100000000, 1);
//	manager->addScene(new RenderQuad(STEREO_SIDEBYSIDE), director::currentTime+4, 100000000, 1);

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
