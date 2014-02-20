#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../utility/log.h"
#include "TBO.h"

using namespace std;

class FBO {
private:
//	GLuint depthrenderbuffer;
	bool deltex;
	void shout_error(GLenum error);
	int viewport[4];

public:
	GLsizei width, height;
	int ntbo;
	vector<oldTBO*> textures;
	oldTBO *depthtexture;
	bool status;

	FBO(GLsizei width, GLsizei height, bool bdo, int ntbo, bool *qualite);

	FBO(GLsizei width, GLsizei height, vector<oldTBO*> texs, oldTBO *depth, bool *qualite);

	void bind(bool erase=true);

	void unbind();

	void erase();

	void bind_texture(int texture, int id);

	void bind_depth_texture(int id);

	//FBO id
	GLuint theID;
};