#include "Buffer_Object.h"

void Buffer_Object::destroy()
{
	this-> ~Buffer_Object();
}

Buffer_Object::~Buffer_Object()
{
	glDeleteBuffers(1, &theID);
}