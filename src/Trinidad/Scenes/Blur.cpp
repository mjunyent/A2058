#include "Blur.h"
#include "../Director/director.h"

BlurScene::BlurScene(TBO *input, float radius, FBO *output) {
	this->input = input;
	this->radius = radius;
	this->output = output;

	first  = new Shader("Shaders/Post/general.vert", "Shaders/Post/BlurFirst.frag");

	bool qualite[1] = {true};
	impas = new FBO(input->width, input->height, false, 1, qualite);

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
		input->bind(0);
		glUniform1i(texID, 0);
		glUniform1i(OrientationID, 0);
		glUniform1i(BlurAmountID, 10);
		glUniform1f(BlurStrengthID, 0.0);
		glUniform2f(texelSizeID, 1.0/float(input->width), 1.0/float(input->height));

		quad->enable(3);
		quad_I->draw(GL_TRIANGLES);
		quad->disable();
	impas->unbind();

	//Second Pass
	if(output != NULL) output->bind();
		impas->bind_texture(0, 0);
		glUniform1i(texID, 0);
		glUniform1i(OrientationID, 1);

		quad->enable(3);
		quad_I->draw(GL_TRIANGLES);
		quad->disable();
	if(output != NULL) output->unbind();

	glEnable(GL_DEPTH_TEST);
}
