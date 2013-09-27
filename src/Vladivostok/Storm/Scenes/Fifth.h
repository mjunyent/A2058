#pragma once
#ifndef STORM_FIFTH
#define STORM_FIFTH

#include "StormScene.h"

class FifthStormScene : public StormScene {
public:
	VBO *textQuad, *linesQuad, *evoQuad;
	TBO text, squareLines;
	vector<TBO*> evolution;

	//Parameters
	float textHeight;
	float linesHeight;
	float evoHeight;
	vec3 evoTranslate;

	//State Vars.
	int evoFrame;
	STATE last;
	float saveDelRad;
	float saveScanSize;
	float evoWidth;

	FifthStormScene(CSParser *csp, Scanner *s);

	void renderModel();
	void textDraw(mat4 *V, mat4 *P, FBO *render);
	void linesDraw(mat4 *V, mat4 *P, FBO *render);
	void modelDraw(mat4 *V, mat4 *P, FBO *render, bool left);

	STATE flowControl();

	void update();

	void readConf(CSParser *csp);
};

#endif