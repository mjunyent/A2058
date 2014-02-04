#include "Buffer_Object.h"

BO::BO(GLenum type) {
	glGenBuffers(1, &id);
	glBindBuffer(type, id);
	this->type = type;
}

BO::BO(BO *bo) {
	glGenBuffers(1, &id);
	glBindBuffer(bo->type, id);
	this->type = bo->type;
	this->size = bo->size;
	this->usage = bo->usage;

	glBufferData(type, size, NULL, usage);

	copy(bo);
}

BO::~BO() {
	glDeleteBuffers(1, &id);
}

void BO::bind() {
	glBindBuffer(type, id);
}

void BO::bind(GLenum target) {
	glBindBuffer(target, id);
}

void BO::data(const void* data, GLsizeiptr size, GLenum usage) {
	bind();
	this->usage = usage;
	this->size = size;
	glBufferData(type, size, data, usage);
}

void BO::copy(BO *from) {
	from->bind(GL_COPY_READ_BUFFER);
	this->bind(GL_COPY_WRITE_BUFFER);

	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);

	if(glGetError() != 0) Tobago.log->write(ERROR) << "BO copy error";
}

//http://www.opengl.org/wiki/GLAPI/glMapBufferRange
void* BO::map(IOType io) {
	bind();

	GLbitfield access;
	switch (io)
	{
	case BO::READ:
		access = GL_MAP_READ_BIT;
		break;
	case BO::WRITE:
		access = GL_MAP_WRITE_BIT;
		break;
	case BO::RW:
		access = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT;
		break;
	}

	return glMapBufferRange(type, 0, size, access);
}

void BO::unmap() {
	glUnmapBuffer(type);
}





void Buffer_Object::destroy()
{
	this-> ~Buffer_Object();
}

Buffer_Object::~Buffer_Object()
{
	glDeleteBuffers(1, &theID);
}