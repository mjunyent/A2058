#pragma once
#ifndef TRINIDAD_BLUR
#define TRINIDAD_BLUR

#include "Tobago/Tobago.h"
#include "../Director/SceneManager.h"

using namespace std;

class BlurScene : public Scene {
public:
	BlurScene(int width, int height, int radius, float strength);

	void draw(int s, double time);

	int radius;
	float strength;

private:
	int w, h;
	FBO *impas;
	FBO *outputL;
	FBO *outputR;
	Shader *first;

	GLint texID, texelSizeID, OrientationID, BlurAmountID, BlurStrengthID;

	VBO *quad;
	IBO *quad_I;
};

#endif
