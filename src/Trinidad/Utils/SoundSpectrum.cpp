#include "SoundSpectrum.h"

SoundSpectrum::SoundSpectrum() {
	theShad = new Shader("Shaders/spectrum.vert", "Shaders/spectrum.frag");
}

void SoundSpectrum::update(double t) {
	global::song->getSpectrum();
	global::song->getWave();
}

void SoundSpectrum::draw(double t) {
	GLfloat YAY[2048];
	float max = 0;
	float maxID = 0;

	for(int i=0; i<1024; i++) {
		YAY[2*i+1] = global::song->WAVE[i] + 1;
		YAY[2*i] = 2*float(i)/1024.0 - 1;
		if(global::song->WAVE[i] > max) {
			max = global::song->WAVE[i];
			maxID = i;
		}
	}

	VBO ftw(YAY, sizeof(YAY), 0);

	theShad->use();
	ftw.enable(2);
	ftw.draw(GL_LINE_STRIP);
	ftw.disable();

	glEnable(GL_DEPTH_TEST);
	ftw.destroy();
}
