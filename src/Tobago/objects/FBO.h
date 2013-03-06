#pragma once
#include <GL/glew.h>
#include <GL/glfw.h>
#include "Buffer_Object.h"
#include "TBO.h"
#include "../basic/global.h"
#include <iostream>

using namespace std;

//Modificarla para poder a�adir Coloratachments y depthbuffers y pollas como te de la gana, sin l�mite.
class FBO {
private:
//	GLuint depthrenderbuffer;
	GLsizei width, height;
	bool deltex;
	void shout_error(GLenum error);

public:
	int ntbo;
	vector<TBO*> textures;
	TBO *depthtexture;
	bool status;

	FBO(GLsizei width, GLsizei height, bool bdo, int ntbo, bool *qualite);

	FBO(GLsizei width, GLsizei height, vector<TBO*> texs, TBO *depth, bool *qualite);

	void bind();

	void unbind();

	void erase();

	void bind_texture(int texture, int id);

	//FBO id
	GLuint theID;
};