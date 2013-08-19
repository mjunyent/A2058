/*#pragma once
#ifndef TRINIDAD_GLOW
#define TRINIDAD_GLOW

#include "Tobago/Tobago.h"
#include "../Director/SceneManager.h"

using namespace std;

class GlowScene : public Scene {
public:
	GlowScene(int radius, float strength, int mix, TBO *glowMapL, TBO *glowMapR);

	void draw(int s, double time);

	int radius;
	float strength;

private:
	bool offscreen;
	FBO *impas;
	FBO *outputL;
	FBO *outputR;
	Shader *first;

	GLint texID, texelSizeID, OrientationID, BlurAmountID, BlurStrengthID;

	VBO *quad;
	IBO *quad_I;
};

#endif
*/