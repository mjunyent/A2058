#include "RenderQuad.h"
#include "../Director/director.h"

RenderQuad::RenderQuad(STEREOTYPE st) {
	this->st = st;

	if(this->st == STEREO_NONE) {
		shad = new Shader("Shaders/ScreenTexture.vert", "Shaders/3D/No3D.frag");
		LeftID = shad->getUniform("LeftTex");
	} else if(this->st == STEREO_ANAGLYPH_RC) {
		shad = new Shader("Shaders/ScreenTexture.vert", "Shaders/3D/AnaglyphRC.frag");
		LeftID = shad->getUniform("LeftTex");
		RightID = shad->getUniform("RightTex");
	}

	quad = new VBO(director::quad, sizeof(director::quad), 0);
	quad_I = new IBO(director::quad_I, sizeof(director::quad_I));
}

void RenderQuad::draw(int s, double time) {
	glDisable(GL_DEPTH_TEST);
	shad->use();

	inputBuffL->bind_texture(0, 0);
	glUniform1i(LeftID, 0);

	if(st != STEREO_NONE) {
		inputBuffR->bind_texture(0, 1);
		glUniform1i(RightID, 1);
	}

	quad->enable(3);
	quad_I->draw(GL_TRIANGLES);
	quad->disable();

	glEnable(GL_DEPTH_TEST);

	outputBuffL = inputBuffL;
	outputBuffR = inputBuffR;
}
