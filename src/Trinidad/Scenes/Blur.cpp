#include "Blur.h"
#include "../Director/director.h"

BlurScene::BlurScene(int radius, float strength, bool offscreen) {
	this->radius = radius;
	this->strength = strength;

	first  = new Shader("Shaders/Post/general.vert", "Shaders/Post/BlurFirst.frag");

	int width, height;
	glfwGetWindowSize(director::windows[0], &width, &height);

	bool qualite[1] = {true};
	impas = new FBO(width, height, false, 1, qualite);

	quad = new VBO(director::quad, sizeof(director::quad), 0);
	quad_I = new IBO(director::quad_I, sizeof(director::quad_I));

	texID = first->getUniform("Texture");
	texelSizeID    = first->getUniform("TexelSize");
	OrientationID  = first->getUniform("Orientation");
	BlurAmountID   = first->getUniform("BlurAmount");
	BlurStrengthID = first->getUniform("BlurStrength");
}

void BlurScene::draw(int s, double time) {
	if(s != 0) return;

	glDisable(GL_DEPTH_TEST);

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
//	if(output != NULL) output->bind();
		impas->bind_texture(0, 0);
		glUniform1i(texID, 0);
		glUniform1i(OrientationID, 1);

		quad->enable(3);
		quad_I->draw(GL_TRIANGLES);
		quad->disable();
//	if(output != NULL) output->unbind();

	glEnable(GL_DEPTH_TEST);
}
