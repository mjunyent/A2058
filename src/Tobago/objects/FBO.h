#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../utility/log.h"
#include "TBO.h"

using namespace std;

class FBO {
public:
	enum FBOAttachment { DEPTH, STENCIL, COLOR0, COLOR1, COLOR2, COLOR3, COLOR4, COLOR5, COLOR6, COLOR7, COLOR8, COLOR9, COLOR10, COLOR11, COLOR12, COLOR13, COLOR14, COLOR15 };

	FBO();
	void bind();
	void unbind();

	void setViewPort(int width, int height);

	void addTexture(Texture* t, FBOAttachment type, int mipmapLevel=0, int layer=0);
	void addDepthTexture(Texture* t);
	void addStencilTexture(Texture* t);

	GLuint id;
	int width, height;
	//Texture 0 is Depth, 1 is Stencil, 2+ color attachments.
	vector<Texture*> textures;
};

class oldFBO {
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

	oldFBO(GLsizei width, GLsizei height, bool bdo, int ntbo, bool *qualite);

	oldFBO(GLsizei width, GLsizei height, vector<oldTBO*> texs, oldTBO *depth, bool *qualite);

	void bind(bool erase=true);

	void unbind();

	void erase();

	void bind_texture(int texture, int id);

	void bind_depth_texture(int id);

	//FBO id
	GLuint theID;
};