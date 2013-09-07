#pragma once
#ifndef TRINIDAD_DOF
#define TRINIDAD_DOF

#include "../Corella/Camera.h"
#include "../Director/SceneManager.h"
#include "Tobago/Tobago.h"

//The input FBO must have a depth texture.
class DOFScene : public Scene {
public:
	DOFScene(Camera *cam);

	void draw(int s, double time);
	void render(int s, double time);

private:
	Camera *cam;
	FBO *left, *right, *mid, *currentIn, *currentOut;
	Shader *DOFShad;

	GLint DOFTextID, DOFDepthID, DOFBlurCoeffID, DOFFDistID, DOFFarID, DOFNearID, DOFOrientationID, DOFwidthID, DOFheightID;

	GLint LeftID, RightID;

	VBO *quad;
	IBO *quad_I;
};

#endif
