#pragma once
#ifndef TRINIDAD_BLUR
#define TRINIDAD_BLUR

#include "Tobago/Tobago.h"
#include "../Director/SceneManager.h"

using namespace std;

class BlurScene : public Scene {
public:
	BlurScene(TBO *input, float radius, FBO *output = NULL);

	void draw(int s, double time);

	FBO *output;

private:
	TBO *input;
	FBO *impas;
	float radius;
	Shader *first;

	GLint texID, texelSizeID, OrientationID, BlurAmountID, BlurStrengthID;

	VBO *quad;
	IBO *quad_I;
};

#endif
