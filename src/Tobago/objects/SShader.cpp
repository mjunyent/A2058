#include "SShader.h"

SShader::SShader(const GLchar *vertex, const GLchar *fragment)
{
	vert = glCreateShader(GL_VERTEX_SHADER);
	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vert,1,&vertex,NULL);
	glShaderSource(frag,1,&fragment,NULL);
	glCompileShader(vert);
	glCompileShader(frag);
	p = glCreateProgram();
	glAttachShader(p,vert);
	glAttachShader(p,frag);
	glLinkProgram(p);	
}

void SShader::use()
{
	glUseProgram(p);	
}