#include "VBO.h"

VBO::VBO( GLfloat *theArray, GLuint theSizeof , GLuint theIx)
{
	theBytes = theSizeof;
	theFloats = (GLfloat*)malloc(theBytes);
	memcpy(theFloats , theArray, theBytes);
	theIndex = theIx;
	BUFFER = GL_ARRAY_BUFFER;
	TYPE = GL_FLOAT;

	glGenBuffers(1, &this->theID);
    glBindBuffer( BUFFER , theID);
    glBufferData( BUFFER, theBytes, &this->theFloats[0], GL_STATIC_DRAW);
}

VBO::VBO( std::vector<float> theArray, GLuint theIx)
{
	theIndex = theIx;
	BUFFER = GL_ARRAY_BUFFER;
	TYPE = GL_FLOAT;
	theBytes = theArray.size() * sizeof( GLfloat );

	glGenBuffers(1, &this->theID);
    glBindBuffer( BUFFER , theID);
	glBufferData( BUFFER, theArray.size()*sizeof(float), &theArray[0], GL_STATIC_DRAW);
}

void VBO::enable( GLubyte Rn )
{
	components = Rn;
	//std::cout << "Im enabling the buffer!" << std::endl;
	glEnableVertexAttribArray(theIndex);
    glBindBuffer(BUFFER, theID);
    glVertexAttribPointer(theIndex , Rn, TYPE , GL_FALSE , 0 , (void*)0 );
}

void VBO::disable()
{
	glDisableVertexAttribArray(theIndex);
}

VBO::~VBO()
{
}

void VBO::draw( GLushort theDrawType ) {
	glDrawArrays( theDrawType,0, theBytes / sizeof( GLfloat ) / components );
}
