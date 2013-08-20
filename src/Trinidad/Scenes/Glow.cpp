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

	mixBuffs = new Shader("Shaders/Post/general.vert", "Shaders/Post/GlowMix.frag");
	mixTexA = mixBuffs->getUniform("TexA");
	mixTexB = mixBuffs->getUniform("TexB");
	mixMode = mixBuffs->getUniform("Mode");

	int width, height;
	glfwGetWindowSize(director::windows[0], &width, &height);

	bool qualite[1] = {true};
	filtered = new FBO(width/3, height/3, false, 1, qualite);

	blur = new BlurScene(width/3, height/3, radius, strength);
	blur->inputBuffR = NULL;

	outputL = new FBO(width, height, false, 1, qualite);
	if(glowMapR != NULL) outputR = new FBO(width, height, false, 1, qualite);
	outputBuffR = NULL;
}

void GlowScene::draw(int s, double time) {
	glDisable(GL_DEPTH_TEST);

	//Make GlowMap
	filtered->bind();
	filterGlow->use();
	inputBuffL->textures[0]->bind(0);
	glowMapL->bind(1);
	glUniform1i(filterTexID, 0);
	glUniform1i(filterMapID, 1);
	quad->enable(3);
	quad_I->draw(GL_TRIANGLES);
	quad->disable();
	filtered->unbind();

	//Blur GlowMap
	blur->inputBuffL = filtered;
	blur->draw(0, 0);

	//Mix them
	outputL->bind();
	mixBuffs->use();
	blur->outputBuffL->bind_texture(0, 0);
	inputBuffL->bind_texture(0, 1);

	glUniform1i(mixTexA, 0);
	glUniform1i(mixTexB, 1);
	glUniform1i(mixMode, mix);

	quad->enable(3);
	quad_I->draw(GL_TRIANGLES);
	quad->disable();
	outputL->unbind();

	outputBuffL = outputL;

	if(glowMapR != NULL) {
		filtered->bind();
		filterGlow->use();
		inputBuffR->textures[0]->bind(0);
		glowMapR->bind(1);
		glUniform1i(filterTexID, 0);
		glUniform1i(filterMapID, 1);
		quad->enable(3);
		quad_I->draw(GL_TRIANGLES);
		quad->disable();
		filtered->unbind();

		//Blur GlowMap
		blur->inputBuffL = filtered;
		blur->draw(0, 0);

		//Mix them
		outputR->bind();
		mixBuffs->use();
		blur->outputBuffL->bind_texture(0, 0);
		inputBuffR->bind_texture(0, 1);

		glUniform1i(mixTexA, 0);
		glUniform1i(mixTexB, 1);
		glUniform1i(mixMode, mix);

		quad->enable(3);
		quad_I->draw(GL_TRIANGLES);
		quad->disable();
		outputR->unbind();

		outputBuffR = outputR;
	}

	glEnable(GL_DEPTH_TEST);
}

