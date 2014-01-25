#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Tobago/Init/ContextGLFW.h"

int main(void) {
	TobagoInitGLFW(4, 1);
	ContextGLFW asdf = ContextGLFW(1280, 720, "Test", NULL, NULL);
	ContextGLFW fdsa = ContextGLFW(1280, 720, "Test2", NULL, NULL);
	vector<Context*> pppp;
	pppp.push_back(&asdf);
	pppp.push_back(&fdsa);
	Tobago.init(pppp);

	Tobago.use(0);

	Shader simple("simple.vert", "simple.frag");

	float quad[] = { 
		-1.0f,  1.0f, 0.0f,	//0 UP, LEFT
		1.0f,  1.0f, 0.0f, //1 UP, RIGHT
		1.0f, -1.0f, 0.0f, //2 DOWN, RIGHT
		-1.0f, -1.0f, 0.0f  //3 DOWN, LEFT
	};

	GLushort quad_I[] = {
		0, 3, 1,
		1, 3, 2
	};

	GLuint vertex_array, vboID, iboID;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, &quad[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*6, &quad_I[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

//	VBO vq(quad, sizeof(float)*12, 0);
//	IBO vqi(quad_I, sizeof(GLushort)*6);

	while(Tobago.enabled(0)) {
		Tobago.use(0);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glViewport(0, 0, Tobago.contexts[0]->width, Tobago.contexts[0]->height);

		simple.use();

		glBindVertexArray(vertex_array);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);

//		vq.enable(3);
//		vqi.draw(GL_TRIANGLES);
//		vq.disable();
		
		Tobago.swap(0);
		if(glfwGetKey(asdf.window, GLFW_KEY_ESCAPE) && Tobago.enabled(1)) Tobago.stop(1);
	}

//	glfwDestroyWindow(init.glfwInit->windows[0]);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
