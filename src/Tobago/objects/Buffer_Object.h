#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Buffer_Object
{
	public :
	//Tipo del buffer , se llena con el adecuado en funcion de que heredada se llame.
	GLuint BUFFER;
	//Tipo de variable , idem.
	GLuint TYPE  ;
	//Los bytes que ocupa el array "nucleo" del objeto , es necesario guardarlo.
	GLuint theBytes;
	//La ID generada del buffer.
	GLuint theID;
	//Destruir el objeto.
	void destroy();
	//Destructor propiamente dicho , no hay por que llamarlo.
	~Buffer_Object();
};