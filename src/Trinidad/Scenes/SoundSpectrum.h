#pragma once
#ifndef TRINIDAD_SOUNDSPECTRUM
#define TRINIDAD_SOUNDSPECTRUM

#include "Tobago/Tobago.h"

class SoundSpectrum : public Scene {
public:
	SoundSpectrum();
	void draw(double time);
	void update(double time);
private:
	Shader *theShad;
};

#endif