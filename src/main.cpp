#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Tobago/Init/ContextGLFW.h"

int main(void) {
	TobagoInitGLFW(3, 3);
	ContextGLFW asdf = ContextGLFW(1280, 720, "Test", NULL, NULL);
	ContextGLFW fdsa = ContextGLFW(1280, 720, "Test2", NULL, NULL);
	vector<Context*> pppp;
	pppp.push_back(&asdf);
	pppp.push_back(&fdsa);
	Tobago.init(pppp);
	
	Tobago.log->write(DEBUG) << "PEROQUECOM;O!";

	Tobago.use(0);

	Shader simple;
	simple.loadFromFile(GL_VERTEX_SHADER, "simple.vert");
	simple.loadFromFile(GL_FRAGMENT_SHADER, "simple.frag");
	simple.link();
	simple.addUniform("a");
	simple.addUniform("cosa");

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

	GLushort quad_II[] = {
		0, 3, 1,
		1, 3, 2
	};

	float quadSep[] = {
		-1.0f,  1.0f,  0.0f, //0 UP, LEFT		0
		1.0f,  1.0f,  0.0f, //1 UP, RIGHT		1
		-1.0f, -1.0f,  0.0f,  //3 DOWN, LEFT	2

		1.0f,  1.0f,  0.0f, //1 UP, RIGHT		0
		1.0f, -1.0f,  0.0f, //2 DOWN, RIGHT		1
		-1.0f, -1.0f,  0.0f  //3 DOWN, LEFT		2
	};

	GLushort sepQI[] = {
		0,2,1,
		0,2,1
	};

	VAO vaoVAO(GL_TRIANGLES);
	VBO vboBO(quadSep, 18);
	IBO iboBO(sepQI, 6);
	BO mene(&vboBO);

	vaoVAO.addAttribute(0, 3, &vboBO);
	vaoVAO.addIBO(&iboBO);

	vector<GLsizei> ss, vv;
	ss.push_back(3);
	ss.push_back(3);

	vv.push_back(0);
	vv.push_back(3);

	vaoVAO.setMultiDrawElementsNumberOfIndices(ss, vv);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	Tobago.log->flush();

	while(Tobago.enabled(0)) {
		Tobago.use(0);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glViewport(0, 0, Tobago.contexts[0]->width, Tobago.contexts[0]->height);
		float* ray = (float*)vboBO.map(BO::RW);
		ray[3] = sin(glfwGetTime());
		ray[4] = cos(glfwGetTime());
		vboBO.unmap();
		
		simple.use();
		simple("a", 1.0f);
		simple("cosa", 1.0f);

		vaoVAO.multiDrawElements();
//		vaoVAO.draw();
//		vaoVAO.drawInstanced(2);
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
		
		Tobago.swap(0);
		if(glfwGetKey(asdf.window, GLFW_KEY_ESCAPE) && Tobago.enabled(1)) Tobago.stop(1);
	}


//	glfwDestroyWindow(init.glfwInit->windows[0]);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
