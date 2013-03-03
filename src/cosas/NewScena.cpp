#include "cosas/NewScena.h"

NewScena::NewScena( glm::mat4* MVP) {
	this->MVP = MVP;

	pint = new Shader("Shaders/plano.vert","Shaders/Fractal.frag");
	MVP_Id = pint->getUniform("MVP");
	

}

void NewScena::update(double t) {
	
}

void NewScena::draw(double t) {
	VBO pla =  VBO(global::quad,sizeof(global::quad),0);
	IBO pla_i =  IBO(global::quad_I,sizeof(global::quad_I));

	pint->use();
	glUniformMatrix4fv(MVP_Id, 1, GL_FALSE, &(*MVP)[0][0]);
	pla.enable(3);
	//pla.draw(GL_TRIANGLE_STRIP);
	pla_i.draw(GL_TRIANGLES);
	pla.disable();

	glEnable(GL_DEPTH_TEST);
	pla.destroy();
}