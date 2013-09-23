#pragma once
#ifndef STORM_FIRST
#define STORM_FIRST

#include "StormScene.h"

class FirstRenderer : public Deferred {
public:
	A3dsHandler *World_3DS, *Polio_3DS;
	Model *World, *Polio;
	mat4 World_M, Polio_M, rotate_M;
	float WorldSize, PolioSize;


	FirstRenderer(CSParser *csp, Camera *cam);
	void render(int s, double t);

	void setPosition(vec3 *position);

	void readConf(CSParser *csp);
};


class FirstStormScene : public StormScene {
public:
	FirstRenderer *renderF;
	VBO *textQuad, *linesQuad;
	TBO worldText, polioText;

	bool firstStill;

	//Parameters
	float textHeight;
	float linesHeight;
	float stillTime;

	FirstStormScene(CSParser *csp, Scanner *s);

	void renderModel();
	void textDraw(mat4 *V, mat4 *P, FBO *render);
	void linesDraw(mat4 *V, mat4 *P, FBO *render);
	void modelDraw(mat4 *V, mat4 *P, FBO *render, bool left);

	STATE flowControl();

	void readConf(CSParser *csp);

};

#endif