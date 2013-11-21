#pragma once
#ifndef TRINIDAD_GLOW
#define TRINIDAD_GLOW

#include "Tobago/Tobago.h"
#include "../Director/SceneManager.h"
#include "Blur.h"

using namespace std;

class GlowScene : public Scene {
public:
	GlowScene(int radius, float strength, int mix, TBO *glowMapL, TBO *glowMapR=NULL);

	void draw(int s, double time);

	int radius;
	float strength;
	int mix;

private:
	BlurScene *blur;
	TBO *glowMapL, *glowMapR;
	FBO *filtered;
	FBO *outputL;
	FBO *outputR;

	Shader *filterGlow, *mixBuffs;
	GLint filterTexID, filterMapID;
	GLint mixTexA, mixTexB, mixMode;

	VBO *quad;
	IBO *quad_I;
};

#endif