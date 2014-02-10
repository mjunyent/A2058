#ifndef VAO_H
#define VAO_H

#include "../init/initTobago.h"
#include "VBO.h"
#include "IBO.h"

class VAO
{
public:
	VAO(GLenum mode);
	~VAO();

	void bind();
	void unbind();

	void addAttribute(GLuint index, int dimension, VBO* vbo, int stride = 0, int offset = 0, GLboolean normalized = GL_FALSE);
	void addIntAttribute(GLuint index, int dimension, VBO* vbo, int stride = 0, int offset = 0);
	void addDoubleAttribute(GLuint index, int dimension, VBO* vbo, int stride = 0, int offset = 0);
	void addIBO(IBO *ibo);

	void instancedAttribute(GLuint index, GLuint div = 1);

	void enableAttribute(GLuint index);
	void disableAttribute(GLuint index);

	void drawArrays(GLsizei elements);
	void drawElements();

	void VAO::drawArraysInstanced(GLsizei elements, GLsizei times);
	void VAO::drawElementsInstanced(GLsizei times);

	GLuint id;
	GLenum mode;
	IBO *ibo;
};


/*
	glBindVertexArray

	Enable/disable array access:
		glEnableVertexAttribArray(index);
		glDisableVertexAttribArray(index);


		*/
#endif