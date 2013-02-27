#include "SoundSpectrum.h"

namespace SoundSpectrum {

	Shader *theShad;

	void setup() {
		theShad = new Shader("Shaders/spectrum.vert", "Shaders/spectrum.frag");
	}

	void draw(double time) {
		glDisable(GL_DEPTH_TEST);

		GLfloat YAY[512];
		float max = 0;
		float maxID = 0;

		for(int i=0; i<256; i++) {
			YAY[2*i+1] = global::song->FFT[i];
			YAY[2*i] = 2*float(i)/256.0 - 1;
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

	void update(double time) {
		global::song->getSpectrum();
	}
};