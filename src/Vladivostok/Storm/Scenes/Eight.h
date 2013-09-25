#pragma once
#ifndef STORM_EIGHT
#define STORM_EIGHT

#include "StormScene.h"

class EightRendererRBC : public Deferred {
public:
	A3dsHandler *RBC_3DS;
	Model *RBC;
	mat4 RBC_M, rotate_M;
	float FluSize, zLate, rotateVel;

	NinthRendererFlu(CSParser *csp, Camera *cam);
	void render(int s, double t);
	void update(double t);
	void setPosition(vec3 *position);

	void readConf(CSParser *csp);

	CSParser *csp;
	vec3 *pos;
};

class EightStormScene : public StormScene {
public:
//	NinthRendererFlu *renderFlu;

	Shader *heartShad;
	VBO *textQuad, *linesQuad, *heartQuad;
	TBO text, heart;

	//Parameters
	float textHeight;
	float linesHeight;
	float heartHeight;
	float heartAlphaVel;
	float heartAccel;
	float zLate;

	//State Vars.
	float heartVel;
	float heartPosition;
	float heartAlpha;

	EightStormScene(CSParser *csp, Scanner *s);

	void renderModel();
	void textDraw(mat4 *V, mat4 *P, FBO *render);
	void linesDraw(mat4 *V, mat4 *P, FBO *render);
	void modelDraw(mat4 *V, mat4 *P, FBO *render, bool left);

	STATE flowControl();

	void update();

	void readConf(CSParser *csp);

	GLint heart_M_Id, heart_V_Id, heart_P_Id,
		  heart_sP_Id, heart_Tex_Id, heart_alpha_Id;

};

#endif