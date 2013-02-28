#include "IBO.h"

IBO::IBO( GLushort *theArray , GLuint theSizeof )
{
	theBytes = theSizeof;
	theIndices = (GLushort*)malloc(theBytes);
	memcpy(theIndices,theArray,theBytes);
	theBytes+=4;
	BUFFER = GL_ELEMENT_ARRAY_BUFFER;
	TYPE   = GL_UNSIGNED_SHORT;
	glGenBuffers(1, &this->theID);
    glBindBuffer( BUFFER , theID);
    glBufferData( BUFFER, theSizeof, &this->theIndices[0], GL_STATIC_DRAW);		
}

IBO::IBO( vector<GLushort> theArray)
{
	BUFFER = GL_ELEMENT_ARRAY_BUFFER;
	TYPE   = GL_UNSIGNED_SHORT;
	glGenBuffers(1, &this->theID);
    glBindBuffer( BUFFER , theID);
	theBytes = 2*theArray.size();
    glBufferData( BUFFER, theArray.size()*sizeof(GLushort), &theArray[0], GL_STATIC_DRAW);		
}

void IBO::draw( GLushort theDraw)
{
	glBindBuffer(BUFFER, theID);
	glDrawElements(theDraw,theBytes/2,TYPE,(void*)0 );
}

IBO::~IBO()
{

}