#pragma once
#ifndef STORM_FOURTH
#define STORM_FOURTH

#include "StormScene.h"

class FourthRendererFish : public Deferred {
public:
	A3dsHandler *Fish_3DS;
	Model *Fish;
	mat4 Fish_M, rotate_M;
	float FishSize;


	FourthRendererFish(CSParser *csp, Camera *cam);
	void render(int s, double t);

	void setPosition(vec3 *position);

	void readConf(CSParser *csp);
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

	void readConf(CSParser *csp);
};

#endif