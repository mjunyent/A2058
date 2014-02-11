#include "VAO.h"

VAO::VAO(GLenum mode) {
	glGenVertexArrays(1, &id);
	ibo = NULL;
	this->mode = mode;
	elements = -1;
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
	if(elements < 0) elements = vbo->elements;
	glVertexAttribPointer(index, dimension, vbo->type, normalized, stride, (void*)(vbo->sizeofelement*offset));
	glEnableVertexAttribArray(index);
}

void VAO::addIntAttribute(GLuint index, int dimension, VBO* vbo, int stride /* = 0 */, int offset /* = 0 */) {
	bind();
	vbo->bind();
	if(elements < 0) elements = vbo->elements;
	glVertexAttribIPointer(index, dimension, vbo->type, stride, (void*)(vbo->sizeofelement*offset));
	glEnableVertexAttribArray(index);
}

void VAO::addDoubleAttribute(GLuint index, int dimension, VBO* vbo, int stride /* = 0 */, int offset /* = 0 */) {
	bind();
	vbo->bind();
	if(elements < 0) elements = vbo->elements;
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

void VAO::setDrawArraysNumberOfElements(GLsizei elements) {
	this->elements = elements;
}

void VAO::setMultiDrawArraysNumberOfElements(vector<GLsizei>& multipleElements) {
	arraysCount = multipleElements;
	arraysFirst.push_back(0);

	for(int i=0; i<arraysCount.size()-1; i++) {
		arraysFirst.push_back(arraysFirst[i]+arraysCount[i]);
	}
}

void VAO::setMultiDrawElementsNumberOfIndices(vector<GLsizei>& multipleIndices) {
//TODO	this->multipleIndices = multipleIndices;
}

void VAO::drawArrays() {
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

void VAO::multiDrawArrays() {
	bind();
	glMultiDrawArrays(mode, &arraysFirst[0], &arraysCount[0], arraysFirst.size());
}

void VAO::multiDrawElements() {
//TODO	bind();
}

VAO::~VAO() {
	glDeleteVertexArrays(1, &id);
}
