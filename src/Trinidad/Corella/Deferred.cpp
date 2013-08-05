#include "Deferred.h"

Deferred::Deferred(int width, int height, glm::mat4 *P, glm::mat4 *V, glm::vec3 *cam_position) {
	firstShad  = new Shader("Shaders/Deferred/first.vert",  "Shaders/Deferred/first.frag");
	secondShad = new Shader("Shaders/Deferred/second.vert", "Shaders/Deferred/second.frag");
	bool calite[] = { true, true, true };
	renderBuffer = new FBO(width, height, true, 3, calite);

	this->lights = new Light(secondShad, "lights");
	this->P = P;
	this->V = V;
	this->cam_position = cam_position;

	screen_quad = new VBO(director::quad, sizeof(director::quad), 0);
	screen_quad_I = new IBO(director::quad_I, sizeof(director::quad_I));

	P_Id = firstShad->getUniform("Projection");
	V_Id = firstShad->getUniform("View");

	normalID	= secondShad->getUniform("Normal");
	diffuseID	= secondShad->getUniform("Diffuse");
	specularID	= secondShad->getUniform("Specular");
	depthID		= secondShad->getUniform("Depth");
	invPVID		= secondShad->getUniform("invPV");
	camPosID	= secondShad->getUniform("camera_position");
}

void Deferred::PreFirstPass() {
	glDisable(GL_BLEND);
	renderBuffer->bind();
	firstShad->use();
	glUniformMatrix4fv(V_Id, 1, GL_FALSE, &(*V)[0][0]);
	glUniformMatrix4fv(P_Id, 1, GL_FALSE, &(*P)[0][0]);

}

void Deferred::PostFirstPass() {
	renderBuffer->unbind();
	glEnable(GL_BLEND);
}

void Deferred::SecondPass() {
	glm::mat4 invPV = glm::inverse(*P * *V);  //Maybe do this in update, ALSO: make a CAM/RIG good class.

	glDisable(GL_DEPTH_TEST);

	secondShad->use();
	renderBuffer->bind_texture(0, 0);
	renderBuffer->bind_texture(1, 1);
	renderBuffer->bind_texture(2, 2);
	renderBuffer->bind_depth_texture(3);
	lights->passLightToGPU();

	glUniform1i(normalID, 0);
	glUniform1i(diffuseID, 1);
	glUniform1i(specularID, 2);
	glUniform1i(depthID, 3);

	glUniformMatrix4fv(invPVID, 1, GL_FALSE, &invPV[0][0]);
	glUniform3fv(camPosID, 1, &(*cam_position)[0]);

	screen_quad->enable(3);
	screen_quad_I->draw(GL_TRIANGLES);
	screen_quad->disable();

	glEnable(GL_DEPTH_TEST);
}