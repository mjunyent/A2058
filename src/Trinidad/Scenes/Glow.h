#pragma once
#ifndef TRINIDAD_GLOW
#define TRINIDAD_GLOW

#include "Tobago/Tobago.h"
#include "../Director/SceneManager.h"

using namespace std;

class GlowScene : public Scene {
public:
	GlowScene(int radius, float strength, int mix, TBO *glowMapL, TBO *glowMapR=NULL);

	void draw(int s, double time);

	int radius;
	float strength;
	int mix;

private:
	TBO *glowMapL, *glowMapR;
	FBO *filtered;
	FBO *outputL;
	FBO *outputR;

	Shader *filterGlow;

	GLint filterTexID, filterMapID;

	VBO *quad;
	IBO *quad_I;
};

#endif