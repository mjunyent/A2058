/*#include "glow.h"
#include "../Director/director.h"

BlurScene::BlurScene(int radius, float strength) {
	this->radius = radius;
	this->strength = strength;

	first  = new Shader("Shaders/Post/general.vert", "Shaders/Post/BlurFirst.frag");

	int width, height;
	glfwGetWindowSize(director::windows[0], &width, &height);

	bool qualite[1] = {true};
	impas = new FBO(width, height, false, 1, qualite);
	outputL = new FBO(width, height, false, 1, qualite);
	outputR = new FBO(width, height, false, 1, qualite);

	quad = new VBO(director::quad, sizeof(director::quad), 0);
	quad_I = new IBO(director::quad_I, sizeof(director::quad_I));

	texID = first->getUniform("Texture");
	texelSizeID    = first->getUniform("TexelSize");
	OrientationID  = first->getUniform("Orientation");
	BlurAmountID   = first->getUniform("BlurAmount");
	BlurStrengthID = first->getUniform("BlurStrength");
}

void BlurScene::draw(int s, double time) {
	glDisable(GL_DEPTH_TEST);

	outputBuffL = NULL;
	outputBuffR = NULL;

	if(inputBuffL != NULL) {
		//First Pass
		impas->bind();
			first->use();
			inputBuffL->textures[0]->bind(0);
			glUniform1i(texID, 0);
			glUniform1i(OrientationID, 0);
			glUniform1i(BlurAmountID, radius);
			glUniform1f(BlurStrengthID, strength);
			glUniform2f(texelSizeID, 1.0/float(inputBuffL->textures[0]->width), 1.0/float(inputBuffL->textures[0]->height));

			quad->enable(3);
			quad_I->draw(GL_TRIANGLES);
			quad->disable();
		impas->unbind();

		//Second Pass
		outputL->bind();
			impas->bind_texture(0, 0);
			glUniform1i(texID, 0);
			glUniform1i(OrientationID, 1);

			quad->enable(3);
			quad_I->draw(GL_TRIANGLES);
			quad->disable();
		outputL->unbind();

		outputBuffL = outputL;
	}

	if(inputBuffR != NULL) {
		//First Pass
		impas->bind();
			first->use();
			inputBuffR->textures[0]->bind(0);
			glUniform1i(texID, 0);
			glUniform1i(OrientationID, 0);
			glUniform1i(BlurAmountID, radius);
			glUniform1f(BlurStrengthID, strength);
			glUniform2f(texelSizeID, 1.0/float(inputBuffL->textures[0]->width), 1.0/float(inputBuffL->textures[0]->height));

			quad->enable(3);
			quad_I->draw(GL_TRIANGLES);
			quad->disable();
		impas->unbind();

		//Second Pass
		outputR->bind();
			impas->bind_texture(0, 0);
			glUniform1i(texID, 0);
			glUniform1i(OrientationID, 1);

			quad->enable(3);
			quad_I->draw(GL_TRIANGLES);
			quad->disable();
		outputR->unbind();

		outputBuffR = outputR;
	}

	glEnable(GL_DEPTH_TEST);
}
*/