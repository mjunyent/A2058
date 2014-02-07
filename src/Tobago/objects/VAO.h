#ifndef VAO_H
#define VAO_H

#include "../init/initTobago.h"
#include "VBO.h"
#include "IBO.h"

class VAO
{
public:
	VAO();
	~VAO();

	void bind();
	void unbind();

	void addAttribute(GLuint index, int dimension, VBO* vbo, int stride = 0, int offset = 0, GLboolean normalized = GL_FALSE);
	void addIntAttribute(GLuint index, int dimension, VBO* vbo, int stride = 0, int offset = 0);
	void addDoubleAttribute(GLuint index, int dimension, VBO* vbo, int stride = 0, int offset = 0);
	void addIBO(IBO *ibo);

	void enableAttribute(GLuint index);
	void disableAttribute(GLuint index);

	GLuint id;
	bool hasIBO;
};


/*
	glBindVertexArray

	Enable/disable array access:
		glEnableVertexAttribArray(index);
		glDisableVertexAttribArray(index);


		*/
#endif