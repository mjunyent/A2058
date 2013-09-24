#pragma once
#ifndef STORM_FIRST
#define STORM_FIRST

#include "StormScene.h"

class FirstRendererWorld : public Deferred {
public:
	A3dsHandler *World_3DS;
	Model *World;
	mat4 World_M, rotate_M;
	float WorldSize;
	float zLate;


	FirstRendererWorld(CSParser *csp, Camera *cam);
	void render(int s, double t);

	void setPosition(vec3 *position);

	void readConf(CSParser *csp);
};

class FirstRenderPolio : public Deferred {
public:
	A3dsHandler *Polio_3DS;
	Model *Polio;
	mat4 Polio_M, rotate_M;
	float PolioSize;
	float zLate;


	FirstRenderPolio(CSParser *csp, Camera *cam);
	void render(int s, double t);

	void setPosition(vec3 *position);

	void readConf(CSParser *csp);
};


class FirstStormScene : public StormScene {
public:
	FirstRendererWorld *renderFw;
	FirstRenderPolio *renderFp;
	VBO *textQuad, *linesQuad;
	TBO worldText, polioText;

	bool firstStill;

	//Parameters
	float textHeight;
	float linesHeight;

	FirstStormScene(CSParser *csp, Scanner *s);

	void renderModel();
	void textDraw(mat4 *V, mat4 *P, FBO *render);
	void linesDraw(mat4 *V, mat4 *P, FBO *render);
	void modelDraw(mat4 *V, mat4 *P, FBO *render, bool left);

	STATE flowControl();

	void readConf(CSParser *csp);

};

#endif