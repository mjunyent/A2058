#pragma once
#ifndef TRINIDAD_RENDERQUAD
#define TRINIDAD_RENDERQUAD

#include "Tobago/Tobago.h"
#include "../Director/SceneManager.h"

using namespace std;

enum STEREOTYPE { STEREO_NONE, STEREO_ANAGLYPH_RC };

class RenderQuad : public Scene {
public:
	RenderQuad(STEREOTYPE st);

	void draw(int s, double time);

private:
	STEREOTYPE st;
	Shader *shad;

	GLint LeftID, RightID;

	VBO *quad;
	IBO *quad_I;
};

#endif