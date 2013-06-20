#pragma once
#include "Buffer_Object.h"

using namespace std;

class IBO : public Buffer_Object
{
	public :
	//El conjunto de halfs que leidos de 3 en 3 , forman triangulos.
	GLushort *theIndices;
	IBO() {};
	IBO( GLushort *theArray , GLuint theSizeof );
	IBO( vector<GLushort> theArray );
	void draw( GLushort theDraw );
	~IBO();
};