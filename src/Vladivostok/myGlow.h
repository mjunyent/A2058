#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Director/SceneManager.h"
#include "Trinidad/Scenes/Blur.h"

using namespace std;

class myGlowScene : public Scene {
public:
	myGlowScene(int radius, float strength, int mix, TBO *glowMapL, TBO *glowMapR=NULL);

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