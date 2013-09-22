#pragma once
#ifndef STORM_FIRST
#define STORM_FIRST

#include "StormScene.h"

class FirstRenderer : public Deferred {
public:
	A3dsHandler *World_3DS, *Polio_3DS;
	Model *World, *Polio;
	mat4 World_M, Polio_M, rotate_M;


	FirstRenderer(Camera *cam);
	void render(int s, double t);

	void setPosition(vec3 *position);

};

class FirstStormScene : public StormScene {
public:
	FirstRenderer *renderF;
	float textWidth, textHeight, textOffset;
	VBO *textQuad;
	TBO worldText;

	FirstStormScene(CSParser *csp, Scanner *s);

	void renderModel();
	void textDraw(mat4 *V, mat4 *P, FBO *render);
	void modelDraw(mat4 *V, mat4 *P, FBO *render, bool left);

	void readConf(CSParser *csp);

};

#endif