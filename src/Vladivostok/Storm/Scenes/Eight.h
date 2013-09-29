#pragma once
#ifndef STORM_EIGHT
#define STORM_EIGHT

#include "StormScene.h"

class EightRendererRBC : public Deferred {
public:
	A3dsHandler *RBC_3DS;
	Model *RBC;
	mat4 RBC_M, rotate_M;
	float RBCSize, rotValue;
	vec3 RBCdisp[6];
	vec3 rotVecs[6];
	float rotVals[6];
	float rotVelsAdded[6];

	EightRendererRBC(CSParser *csp, Camera *cam, FBO *rL, FBO *rR);
	void render(int s, double t);
	void update(double t);
	void setPosition(vec3 *position);

	void readConf(CSParser *csp);

	void setRotVals();

	CSParser *csp;
	vec3 *pos;
};

class EightStormScene : public StormScene {
public:
	EightRendererRBC *renderRBC;

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
	bool heartZoom;

	EightStormScene(CSParser *csp, Scanner *s, FBO *rL, FBO *rR);

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