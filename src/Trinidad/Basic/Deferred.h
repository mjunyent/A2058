#pragma once
#ifndef TRINIDAD_DEFERRED
#define TRINIDAD_DEFERRED

#include "Tobago/Tobago.h"

class StartDeferred : public Scene {
public:
	Shader *first;
	FBO *renderBuffer;
	StartDeferred();

	void draw(double t);
};


class EndDeferred : public Scene {
public:
	FBO *renderBuffer;

	EndDeferred(StartDeferred *sd);

	void draw(double t);
};


class RenderDeferred : public Scene {
public:
	RenderDeferred(StartDeferred *sd);

	void draw(double t);
};


class DebugDeferred : public Scene {
public:
	Shader *debug;
	FBO *renderBuffer;
	VBO *squad;
	IBO *squad_I;
	GLuint tex1, tex2, tex3, tex4;

	DebugDeferred(StartDeferred *sd);

	void draw(double t);
};

#endif