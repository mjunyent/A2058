#include "Blur.h"
#include "../Director/director.h"

BlurScene::BlurScene(TBO *input, float radius, FBO *output) {
	this->input = input;
	this->radius = radius;
	this->output = output;

	first  = new Shader("Shaders/Post/general.vert", "Shaders/Post/BlurFirst.frag");
	second = new Shader("Shaders/Post/general.vert", "Shaders/Post/BlurSecond.frag");

	bool qualite[1] = {true};

	impas = new FBO(input->width, input->height, false, 1, qualite);

	quad = new VBO(director::quad, sizeof(director::quad), 0);
	quad_I = new IBO(director::quad_I, sizeof(director::quad_I));

	tex1ID = first->getUniform("Texture");
	tex2ID = second->getUniform("Texture");
}

void BlurScene::draw(int s, double time) {
	if(s != 0) return;

	glDisable(GL_DEPTH_TEST);

	//First Pass
	impas->bind();
		first->use();
		input->bind(0);
		glUniform1i(tex1ID, 0);

		quad->enable(3);
		quad_I->draw(GL_TRIANGLES);
		quad->disable();
	impas->unbind();

	//Second Pass
	if(output != NULL) output->bind();
		second->use();
		impas->bind_texture(0, 0);
		glUniform1i(tex2ID, 0);
		
		quad->enable(3);
		quad_I->draw(GL_TRIANGLES);
		quad->disable();
	if(output != NULL) output->unbind();

	glEnable(GL_DEPTH_TEST);
}
