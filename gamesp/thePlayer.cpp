#include "thePlayer.h"

ThePlayer::ThePlayer()
{
	GLfloat simplesquare[] =
	{
		/*
			Esperando coords del portillo para escalar bien la mierda esta ...
		*/
		0.0f ,
	};
	GLushort simpleindices[] =
	{
		0,1,2,
		1,2,3,
	};
	this->square = VBO();
	this->theIBO = IBO( &simpleindices[0] , sizeof(simpleindices) );
}