#pragma once
#ifndef PRUEVA2
#define PRUEVA2

#include "Tobago/Tobago.h"

class FBOScene : public Scene {
public:
	FBOScene(FBO* fbo);
	void draw(double time);
	void update(double time);
	GLuint fbo_Id;
	FBO *fbo;
private:
	Shader *pint;
	
};

#endif