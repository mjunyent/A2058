#include "glow.h"
#include "../Director/director.h"

GlowScene::GlowScene(int radius, float strength, int mix, TBO *glowMapL, TBO *glowMapR) {
	this->radius = radius;
	this->strength = strength;
	this->mix = mix;
	this->glowMapL = glowMapL;
	this->glowMapR = glowMapR;

	quad = new VBO(director::quad, sizeof(director::quad), 0);
	quad_I = new IBO(director::quad_I, sizeof(director::quad_I));

	filterGlow = new Shader("Shaders/Post/general.vert", "Shaders/Post/GlowFilter.frag");
	filterTexID = filterGlow->getUniform("Texture");
	filterMapID = filterGlow->getUniform("Map");

	int width, height;
	glfwGetWindowSize(director::windows[0], &width, &height);

	bool qualite[1] = {true};
	filtered = new FBO(width, height, false, 1, qualite);
}

void GlowScene::draw(int s, double time) {
	glDisable(GL_DEPTH_TEST);

	filterGlow->use();

	inputBuffL->textures[0]->bind(0);
	glowMapL->bind(1);

	glUniform1i(filterTexID, 0);
	glUniform1i(filterMapID, 1);

	quad->enable(3);
	quad_I->draw(GL_TRIANGLES);
	quad->disable();

	glEnable(GL_DEPTH_TEST);
}

