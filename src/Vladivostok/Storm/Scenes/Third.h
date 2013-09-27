#pragma once
#ifndef STORM_THIRD
#define STORM_THIRD

#include "StormScene.h"

class ThirdStormScene : public StormScene {
public:
	VBO *textQuad, *linesQuad, *jellyQuad;
	TBO text;
	vector<TBO*> jelly;

	//Parameters
	float textHeight;
	float linesHeight;
	float jellyHeight;
	vec3 position, displacementVec;
	float displacementVel;

	//State Vars.
	int jellyFrame;
	bool skip;
	float dispPos;

	ThirdStormScene(CSParser *csp, Scanner *s);

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