#pragma once
#ifndef STORM_NINTH
#define STORM_NINTH

#include "StormScene.h"

class NinthRendererFlu : public Deferred {
public:
	A3dsHandler *Flu_3DS;
	Model *Flu;
	mat4 Flu_M, rotate_M;
	float FluSize, zLate, rotateVel;

	NinthRendererFlu(CSParser *csp, Camera *cam, FBO *rL, FBO *rR);
	void render(int s, double t);
	void update(double t);
	void setPosition(vec3 *position);

	void readConf(CSParser *csp);

	CSParser *csp;
	vec3 *pos;
};

class NinthStormScene : public StormScene {
public:
	NinthRendererFlu *renderFlu;
//	FourthRendererFish *renderFish;

	VBO *textQuad, *linesQuad;
	TBO text;

	//Parameters
	float textHeight;
	float linesHeight;

	NinthStormScene(CSParser *csp, Scanner *s, FBO *rL, FBO *rR);

	void renderModel();
	void textDraw(mat4 *V, mat4 *P, FBO *render);
	void linesDraw(mat4 *V, mat4 *P, FBO *render);
	void modelDraw(mat4 *V, mat4 *P, FBO *render, bool left);

	void update();

	STATE flowControl();

	void readConf(CSParser *csp);
};

#endif