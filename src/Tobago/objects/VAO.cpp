#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &id);
	hasIBO = false;
}

void VAO::bind() {
	glBindVertexArray(id);
}

void VAO::unbind() {
	glBindVertexArray(0);
}

void VAO::addAttribute(GLuint index, int dimension, VBO* vbo, int stride /* = 0 */, int offset /* = 0 */, GLboolean normalized /* = GL_FALSE */) {
	bind();
	vbo->bind();
	glVertexAttribPointer(index, dimension, vbo->type, normalized, stride, (void*)(vbo->sizeofelement*offset));
	glEnableVertexAttribArray(index);
}

void VAO::addIntAttribute(GLuint index, int dimension, VBO* vbo, int stride /* = 0 */, int offset /* = 0 */) {
	bind();
	vbo->bind();
	glVertexAttribIPointer(index, dimension, vbo->type, stride, (void*)(vbo->sizeofelement*offset));
	glEnableVertexAttribArray(index);
}

void VAO::addDoubleAttribute(GLuint index, int dimension, VBO* vbo, int stride /* = 0 */, int offset /* = 0 */) {
	bind();
	vbo->bind();
	glVertexAttribLPointer(index, dimension, vbo->type, stride, (void*)(vbo->sizeofelement*offset));
	glEnableVertexAttribArray(index);
}

void VAO::addIBO(IBO *ibo) {
	hasIBO = true;
	bind();
	ibo->bind();
}

void VAO::enableAttribute(GLuint index) {
	glEnableVertexAttribArray(index);
}

void VAO::disableAttribute(GLuint index) {
	glDisableVertexAttribArray(index);
}

VAO::~VAO() {
	glDeleteVertexArrays(1, &id);
}
