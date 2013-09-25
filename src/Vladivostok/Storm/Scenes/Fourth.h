#pragma once
#ifndef STORM_FOURTH
#define STORM_FOURTH

#include "StormScene.h"

class FourthRendererFish : public Deferred {
public:
	A3dsHandler *Fish_3DS, *Eye_3DS;
	Model *Fish, *Eye;
	mat4 Fish_M, rotate_M, Eye_M;
	float FishSize, EyeSize;
	float rotateAlpha, rotateVel;
	vec3 eyePos;
	vec3 rotateV, transV;

	FourthRendererFish(CSParser *csp, Camera *cam);
	void render(int s, double t);
	void update(double t);
	void setPosition(vec3 *position);

	void readConf(CSParser *csp);

	CSParser *csp;
	vec3 *pos;
};

class FourthStormScene : public StormScene {
public:
	FourthRendererFish *renderFish;

	VBO *textQuad, *linesQuad;
	TBO text;

	//Parameters
	float textHeight;
	float linesHeight;

	FourthStormScene(CSParser *csp, Scanner *s);

	void renderModel();
	void textDraw(mat4 *V, mat4 *P, FBO *render);
	void linesDraw(mat4 *V, mat4 *P, FBO *render);
	void modelDraw(mat4 *V, mat4 *P, FBO *render, bool left);

	STATE flowControl();

	void update();

	void readConf(CSParser *csp);
};

#endif