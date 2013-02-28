#include "GL/glew.h"
#include "GL/glfw.h"
#pragma once

class SShader
{
	public :
		GLuint p , vert , frag;
		SShader(){};
		SShader(const GLchar *vertex, const GLchar *fragment);
		void use();
};
