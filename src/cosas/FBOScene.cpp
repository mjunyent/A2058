#include "cosas/FBOScene.h"

FBOScene::FBOScene(FBO* fbo) {
	this->fbo = fbo;
	pint = new Shader("Shaders/fboshader.vert","Shaders/fboshader.frag");
	fbo_Id  = pint->getUniform("tex");

}

void FBOScene::update(double t) {
	
}

void FBOScene::draw(double t) {
	VBO pla =  VBO(global::quad,sizeof(global::quad),0);
	IBO pla_i =  IBO(global::quad_I,sizeof(global::quad_I));

	pint->use();
	fbo->bind_texture(0);
	glUniform1i(fbo_Id, 0);
	pla.enable(3);
	//pla.draw(GL_TRIANGLE_STRIP);
	pla_i.draw(GL_TRIANGLES);
	pla.disable();

	glEnable(GL_DEPTH_TEST);
	pla.destroy();
}