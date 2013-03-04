#include "RenderToBuffer.h"

BindFBO::BindFBO(FBO *fbo) {
	this->fbo = fbo;
}

void BindFBO::draw(double t) {
	fbo->bind();
}



UnbindFBO::UnbindFBO(FBO *fbo) {
	this->fbo = fbo;
}

void UnbindFBO::draw(double t) {
	fbo->unbind();
}



SimpleFBORender::SimpleFBORender(FBO *fbo) {
	this->fbo = fbo;
	shad = new Shader("Shaders/ScreenTexture.vert", "Shaders/ScreenTexture.frag");
	tex_ID = shad->getUniform("tex");

	screen_quad = new VBO(global::quad, sizeof(global::quad), 0);
	screen_quad_I  = new IBO(global::quad_I, sizeof(global::quad_I));
}

void SimpleFBORender::draw(double t) {
	glDisable(GL_DEPTH_TEST);
	shad->use();

	fbo->bind_texture(0);
	glUniform1i(tex_ID, 0);

	screen_quad->enable(3);
	screen_quad_I->draw(GL_TRIANGLES);
	screen_quad->disable();

	glEnable(GL_DEPTH_TEST);
}