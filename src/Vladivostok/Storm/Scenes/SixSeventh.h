#pragma once
#ifndef STORM_SIXSEVENTH
#define STORM_SIXSEVENTH

#include "StormScene.h"

class SixthRendererSkull : public Deferred {
public:
	A3dsHandler *Skull_3DS;
	Model *Skull;
	mat4 Skull_M;
	float SkullSize;
	float zLate;

	SixthRendererSkull(CSParser *csp, Camera *cam);
	void render(int s, double t);

	void setPosition(vec3 *position);

	void readConf(CSParser *csp);

	void update(double t);

	CSParser *csp;
	vec3 *pos;
};

class SeventhRendererBrain : public Deferred {
public:
	A3dsHandler *Brain_3DS;
	Model *Brain;
	mat4 Brain_M, rotate_M;
	float BrainSize;
	float zLate;
	float rotationVel;

	SeventhRendererBrain(CSParser *csp, Camera *cam);
	void render(int s, double t);

	void setPosition(vec3 *position);

	void readConf(CSParser *csp);

	void update(double t);

	CSParser *csp;
	vec3 *pos;
};


class SixSeventhStormScene : public StormScene {
public:
//	FirstRendererWorld *renderFw;
//	FirstRenderPolio *renderFp;
	VBO *textQuad, *linesQuad;
	TBO skullText, brainText;

	bool firstStill;

	//Parameters
	float textHeight;
	float linesHeight;

	SixSeventhStormScene(CSParser *csp, Scanner *s);

	void renderModel();
	void textDraw(mat4 *V, mat4 *P, FBO *render);
	void linesDraw(mat4 *V, mat4 *P, FBO *render);
	void modelDraw(mat4 *V, mat4 *P, FBO *render, bool left);

	STATE flowControl();

	void update();

	void readConf(CSParser *csp);

};

#endif