#include "VAO.h"

VAO::VAO(GLenum mode) {
	glGenVertexArrays(1, &id);
	ibo = NULL;
	this->mode = mode;
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
	this->ibo = ibo;
	bind();
	ibo->bind();
}

void VAO::instancedAttribute(GLuint index, GLuint div /* = 1 */) {
	bind();
	glVertexAttribDivisor(index, div); 
}

void VAO::enableAttribute(GLuint index) {
	bind();
	glEnableVertexAttribArray(index);
}

void VAO::disableAttribute(GLuint index) {
	bind();
	glDisableVertexAttribArray(index);
}

void VAO::drawArrays(GLsizei elements) {
	bind();
	glDrawArrays(mode, 0, elements);
}

void VAO::drawElements() {
	bind();
	glDrawElements(mode, ibo->elements, ibo->type, (void*)0);
}

void VAO::drawArraysInstanced(GLsizei elements, GLsizei times) {
	bind();
	glDrawArraysInstanced(mode, 0, elements, times);
}

void VAO::drawElementsInstanced(GLsizei times) {
	bind();
	glDrawElementsInstanced(mode, ibo->elements, ibo->type, (void*)0, times);

} 

VAO::~VAO() {
	glDeleteVertexArrays(1, &id);
}
