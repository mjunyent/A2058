#pragma once

#include "Buffer_Object.h"
#include <cstdlib>
#include <vector>

class NewVBO {
public:
	NewVBO();

	void populate(std::vector<float>& data);

	GLuint id;
};

class VBO : public Buffer_Object
{
	public :
	//El conjunto de floats que , leidos de 3 en 3 , forman los vertices.
	GLfloat *theFloats;
	//El indice del buffer.
	GLuint theIndex;
	VBO() {};
	VBO( GLfloat *theArray, GLuint theSizeof , GLuint theIx );
	VBO( std::vector<float> theArray, GLuint theIx );
	//Habilitar puntero.
	void enable( GLubyte Rn );
	void draw( GLushort );
	//Deshabilitar puntero.
	void disable();
	~VBO();

	int components;
};
