#include "SoundSpectrum.h"

SoundSpectrum::SoundSpectrum() {
	theShad = new Shader("Shaders/spectrum.vert", "Shaders/spectrum.frag");
}

void SoundSpectrum::update(double t) {
	global::song->getSpectrum();
//	global::song->getWave();
}

void SoundSpectrum::draw(double t) {
	const int size = 40;
	GLfloat YAY[2*size];
	float max = 0;
	float maxID = 0;

	for(int i=0; i<size; i++) {
		YAY[2*i+1] = global::song->FFT[i];
		YAY[2*i] = 2*float(i)/float(size) - 1;
		if(global::song->FFT[i] > max) {
			max = global::song->FFT[i];
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
