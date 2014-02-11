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

	void setDrawArraysNumberOfElements(GLsizei elements);
	void setMultiDrawArraysNumberOfElements(vector<GLsizei>& multipleElements);
	void setMultiDrawElementsNumberOfIndices(vector<GLsizei>& multipleIndices);

	void draw();
	void drawArrays();
	void drawElements();

	void drawInstanced(GLsizei times);
	void drawArraysInstanced(GLsizei times);
	void drawElementsInstanced(GLsizei times);

	void multiDrawArrays();
	void multiDrawElements();

	GLuint id;
	GLenum mode;
	IBO *ibo;
	GLsizei elements;
	vector<GLsizei> arraysFirst, arraysCount;
	vector<GLsizei> multipleIndices;
};


/*
	glBindVertexArray

	Enable/disable array access:
		glEnableVertexAttribArray(index);
		glDisableVertexAttribArray(index);


		*/
#endif