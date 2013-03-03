#include "NewScena.h"

NewScena::NewScena() {
	theShad = new Shader("plano.vert","normal.frag");
}

void NewScena::update(double t) {
	//global::song->getSpectrum();
}

void NewScena::draw(double t) {
	VBO pla(global::quad,sizeof(global::quad),0);
	IBO pla_i(global::quad_I,sizeof(global::quad_I));
	

	theShad->use();
	pla.enable(2);
	pla.draw(GL_TRIANGLES);
	pla.disable();

	glEnable(GL_DEPTH_TEST);
	pla.destroy();
}