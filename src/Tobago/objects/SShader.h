#include "GL/glew.h"
#include "GLFW/glfw3.h"
#pragma once

class SShader
{
	public :
		GLuint p , vert , frag;
		SShader(){};
		SShader(const GLchar *vertex, const GLchar *fragment);
		void use();
};
