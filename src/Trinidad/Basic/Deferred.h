#pragma once
#ifndef TRINIDAD_DEFERRED
#define TRINIDAD_DEFERRED

#include "Tobago/Tobago.h"

class StartDeferred : public Scene {
public:
	StartDeferred();

	void draw(double t);
};

class EndDeferred : public Scene {
public:
	EndDeferred(StartDeferred *sd);

	void draw(double t);
};

class RenderDeferred : public Scene {
public:
	RenderDeferred(StartDeferred *sd);

	void draw(double t);
};

#endif