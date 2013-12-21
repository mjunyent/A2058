#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.

int main(void) {
	initTobago init(initTobago::useGLFW, 1280, 720, 4, 1, "Test", false);

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

	while(!glfwWindowShouldClose(init.glfwInit->windows[0])) {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		simple.use();

		glBindVertexArray(vertex_array);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);

//		vq.enable(3);
//		vqi.draw(GL_TRIANGLES);
//		vq.disable();
		
		glfwSwapBuffers(init.glfwInit->windows[0]);
		glfwPollEvents();
		if(glfwGetKey(init.glfwInit->windows[0], GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(init.glfwInit->windows[0], GL_TRUE);
	}

	glfwDestroyWindow(init.glfwInit->windows[0]);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
