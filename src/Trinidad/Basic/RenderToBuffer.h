#pragma once
#ifndef TRINIDAD_RENDERTOBUFFER
#define TRINIDAD_RENDERTOBUFFER

#include "Tobago/Tobago.h"

class BindFBO : public Scene {
public:
	FBO *fbo;
	BindFBO(FBO *fbo);
	void draw(double t);
};

class UnbindFBO : public Scene {
public:
	FBO *fbo;
	UnbindFBO(FBO *fbo);
	void draw(double t);
};

class SimpleFBORender : public Scene {
public:
	FBO *fbo;
	Shader *shad;
	VBO *screen_quad;
	IBO *screen_quad_I;
	GLuint tex_ID;
	int fbo_tex_id;

	SimpleFBORender(FBO *fbo, int id);
	void draw(double t);
};

#endif