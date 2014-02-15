#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Tobago/Init/ContextGLFW.h"

int main(void) {
	TobagoInitGLFW(3, 3);

	ContextGLFW asdf = ContextGLFW(1280, 720, "Test", NULL, NULL);
	vector<Context*> pppp;
	pppp.push_back(&asdf);

	Tobago.init(pppp);

	Tobago.use(0);

	Shader simple;
	simple.loadFromFile(GL_VERTEX_SHADER, "simple.vert");
	simple.loadFromFile(GL_FRAGMENT_SHADER, "simple.frag");
	simple.link();
	simple.addUniform("azul");
	simple.addUniform("MVP");

	float quad[] = {
		-1.0f,  1.0f,  0.0f, //0 UP, LEFT
		 1.0f,  1.0f,  0.0f, //1 UP, RIGHT
		 1.0f, -1.0f,  0.0f, //2 DOWN, RIGHT
		-1.0f, -1.0f,  0.0f  //3 DOWN, LEFT
	};

	GLushort quad_I[] = {
		0, 3, 1,
		1, 3, 2
	};

	VBO vboBO(quad, 12);
	IBO iboBO(quad_I, 6);

	VAO vaoVAO(GL_TRIANGLES);
	vaoVAO.addAttribute(0, 3, &vboBO);
	vaoVAO.addIBO(&iboBO);

	glm::mat4 projection = glm::perspective(90.0f, 1280.0f/720.0f, 0.1f, 100.0f);
	
	Tobago.log->flush();

	while(Tobago.enabled(0)) {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glViewport(0, 0, Tobago.contexts[0]->width, Tobago.contexts[0]->height);
		
		glm::mat4 View       = glm::lookAt(
			glm::vec3(sin(glfwGetTime())*4,cos(glfwGetTime())*4,3), // Camera is at (4,3,3), in World Space
			glm::vec3(0,0,0), // and looks at the origin
			glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
			);
		glm::mat4 mvp = projection*View;


		simple.use();
		simple("azul", (float)sin(10.0f*glfwGetTime()));
		simple("MVP", &mvp);
		vaoVAO.draw();

		Tobago.swap(0);
		if(glfwGetKey(asdf.window, GLFW_KEY_ESCAPE) && Tobago.enabled(0)) Tobago.stop(0);
	}

	Tobago.log->flush();
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
