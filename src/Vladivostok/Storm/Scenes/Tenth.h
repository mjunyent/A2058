#pragma once
#ifndef STORM_TENTH
#define STORM_TENTH

#include "StormScene.h"

class TenthStormScene : public StormScene {
public:
	VBO *textQuad, *linesQuad, *logoQuad;
	TBO logo;
	vector<TBO*> text;

	//Parameters
	float textHeight;
	float linesHeight;
	float logoHeight;
	float zLate;
	vec3 logoLate;

	//State Vars.
	int textFrame;
	STATE last;
	float saveDelRad;
	float saveScanSize;

	TenthStormScene(CSParser *csp, Scanner *s);

	void renderModel();
	void textDraw(mat4 *V, mat4 *P, FBO *render);
	void linesDraw(mat4 *V, mat4 *P, FBO *render);
	void modelDraw(mat4 *V, mat4 *P, FBO *render, bool left);

	STATE flowControl();

	void update();

	void readConf(CSParser *csp);
};

#endif