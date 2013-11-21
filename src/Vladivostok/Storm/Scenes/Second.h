#pragma once
#ifndef STORM_SECOND
#define STORM_SECOND

#include "StormScene.h"

class SecondRendererInnerPlacoderm : public Deferred {
public:
	A3dsHandler *Inner_3DS;
	Model *Inner;
	mat4 Inner_M[4], rotate_M[4], other_M;
	float InnerSize, rotVel, zLate, zoomVel, currentZoomVel;
	vec3 rotationVector[4];
	int popCount;
	bool move;

	SecondRendererInnerPlacoderm(CSParser *csp, Camera *cam);
	void render(int s, double t);
	void update(double t);
	void setPosition(vec3 *position);

	void readConf(CSParser *csp);

	void setValues();
	void startMove();

	CSParser *csp;
	vec3 *pos;
};

class SecondRendererOutterPlacoderm : public Deferred {
public:
	A3dsHandler *Inner_3DS;
	Model *Inner;
	mat4 Inner_M, rotate_M;
	float InnerSize, rotVel;
	SecondRendererInnerPlacoderm *in;

	SecondRendererOutterPlacoderm(CSParser *csp, Camera *cam, SecondRendererInnerPlacoderm *in);
	void render(int s, double t);
	void update(double t);
	void setPosition(vec3 *position);

	void readConf(CSParser *csp);

	CSParser *csp;
	vec3 *pos;
};


class SecondStormScene : public StormScene {
public:
	SecondRendererInnerPlacoderm *inner;
	SecondRendererOutterPlacoderm *outter;

	Shader *mixAlphaShad;
	TBO text;
	VBO *linesQuad, *textQuad;

	//Parameters
	float textHeight;
	float linesHeight;
	float alphaVal;

	SecondStormScene(CSParser *csp, Scanner *s);

	void renderModel();
	void textDraw(mat4 *V, mat4 *P, FBO *render);
	void linesDraw(mat4 *V, mat4 *P, FBO *render);
	void modelDraw(mat4 *V, mat4 *P, FBO *render, bool left);

	STATE flowControl();

	void update();

	void readConf(CSParser *csp);

	GLint mix_position_Id, mix_TexIn_Id, mix_TexOut_Id, mix_DepthIn_Id, mix_DepthOut_Id, mix_OutAlpha_Id;
};

#endif