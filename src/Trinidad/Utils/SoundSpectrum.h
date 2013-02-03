#pragma once
#ifndef TRINIDAD_SOUNDSPECTRUM
#define TRINIDAD_SOUNDSPECTRUM

#include "Tobago/Tobago.h"

namespace SoundSpectrum {
	extern Shader *theShad;
	void setup();
	void draw(double time);
	void update(double time);
};

#endif