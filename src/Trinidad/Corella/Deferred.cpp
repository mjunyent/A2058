#include "Deferred.h"

Deferred::Deferred(Camera *cam, int debScreen) {
	this->cam = cam;
	this->debScreen = debScreen;
	setup();
}

void Deferred::setup() {
	//Load Shaders
	firstShad  = new Shader("Shaders/Deferred/first.vert",  "Shaders/Deferred/first.frag");
	secondShad = new Shader("Shaders/Deferred/second.vert", "Shaders/Deferred/second.frag");
	thirdShad  = new Shader("Shaders/Deferred/third.vert",  "Shaders/Deferred/third.frag");
	debugShad  = new Shader("Shaders/Deferred/debug.vert",  "Shaders/Deferred/debug.frag");

	//Prepare FBOs
	bool calite[] = { true, true, true };
	renderBuffer = new FBO(cam->width, cam->height, true, 3, calite);
	bool lecalite[] = { true };
	finalRender = new FBO(cam->width, cam->height, false, 1, calite);

	//Data...
	this->lights = new Light(secondShad, "lights");

	//Our Screen Quad
	screen_quad = new VBO(director::quad, sizeof(director::quad), 0);
	screen_quad_I = new IBO(director::quad_I, sizeof(director::quad_I));

	//First Shade IDs
	P_Id = firstShad->getUniform("Projection");
	V_Id = firstShad->getUniform("View");

	//Second Shader IDs
	normalID	 = secondShad->getUniform("Normal");
	diffuseID	 = secondShad->getUniform("Diffuse");
	specularID	 = secondShad->getUniform("Specular");
	depthID		 = secondShad->getUniform("Depth");
	invPVID		 = secondShad->getUniform("invPV");
	camPosID	 = secondShad->getUniform("camera_position");
	bgColorID	 = secondShad->getUniform("bgColor");
	isBgTexID	 = secondShad->getUniform("isBgTex");
	backgroundID = -1;

	//Third Shader IDs
	finalNID = thirdShad->getUniform("Normal");
	finalDID = thirdShad->getUniform("Depth");
	finalID  = thirdShad->getUniform("Final");
	widthID  = thirdShad->getUniform("width");
	heightID = thirdShad->getUniform("height");

	//Debug setup.
	if(debScreen != -1) {
		tex1ID = debugShad->getUniform("tex1");
		tex2ID = debugShad->getUniform("tex2");
		tex3ID = debugShad->getUniform("tex3");
		tex4ID = debugShad->getUniform("tex4");
		debInvID = debugShad->getUniform("invPersp");
	}

	bg_color = new glm::vec3(0.0f, 0.0f, 0.0f);
}

void Deferred::setBackground(char *name) {
	backgroundID = secondShad->getUniform("Background");
	backgroundTex = TBO(name, true);
}

void Deferred::setBackground(glm::vec3 *color) {
	backgroundID = -1;
	bg_color = color;
}

void Deferred::PreFirstPass() {
	glDisable(GL_BLEND);
	renderBuffer->bind();
	firstShad->use();
	glUniformMatrix4fv(V_Id, 1, GL_FALSE, &cam->V[0][0]);
	glUniformMatrix4fv(P_Id, 1, GL_FALSE, &cam->P[0][0]);

}

void Deferred::PostFirstPass() {
	renderBuffer->unbind();
	glEnable(GL_BLEND);
}

void Deferred::SecondPass() {
	glm::mat4 invPV = glm::inverse(cam->P * cam->V);  //Maybe do this in update, ALSO: make a CAM/RIG good class.

	glDisable(GL_DEPTH_TEST);

	secondShad->use();
	renderBuffer->bind_texture(0, 0);
	renderBuffer->bind_texture(1, 1);
	renderBuffer->bind_texture(2, 2);
	renderBuffer->bind_depth_texture(3);
	lights->passLightToGPU();

	if(backgroundID != -1) {
		backgroundTex.bind(4);
		glUniform1i(isBgTexID, 1);
		glUniform1i(backgroundID, 4);
	} else {
		glUniform1i(isBgTexID, 0);
		glUniform3fv(bgColorID, 1, &(*bg_color)[0]); 
	}

	glUniform1i(normalID, 0);
	glUniform1i(diffuseID, 1);
	glUniform1i(specularID, 2);
	glUniform1i(depthID, 3);

	glUniformMatrix4fv(invPVID, 1, GL_FALSE, &invPV[0][0]);
	glUniform3fv(camPosID, 1, &cam->position[0]);

	screen_quad->enable(3);
	screen_quad_I->draw(GL_TRIANGLES);
	screen_quad->disable();

	glEnable(GL_DEPTH_TEST);
}

void Deferred::ThirdPass() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	thirdShad->use();

	finalRender->bind_texture(0, 0);
	renderBuffer->bind_texture(0, 1);
	renderBuffer->bind_depth_texture(2);

	glUniform1i(finalID, 0);
	glUniform1i(finalNID, 1);
	glUniform1i(finalDID, 2);
	glUniform1f(widthID, cam->width);
	glUniform1f(heightID, cam->height);


	screen_quad->enable(3);
	screen_quad_I->draw(GL_TRIANGLES);
	screen_quad->disable();

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Deferred::draw(int s, double t) {
	PreFirstPass();
	render(s, t);
	PostFirstPass();
	if(s == debScreen) Debug();
	else {
		finalRender->bind();
		SecondPass();
		finalRender->unbind();
		ThirdPass();
	}
}

void Deferred::Debug() {
	glm::mat4 invPV = glm::inverse(cam->P * cam->V);  //Maybe do this in update, ALSO: make a CAM/RIG good class.

	glDisable(GL_DEPTH_TEST);

	debugShad->use();
	renderBuffer->bind_texture(0, 0);
	renderBuffer->bind_texture(1, 1);
	renderBuffer->bind_texture(2, 2);
	renderBuffer->bind_depth_texture(3);

	glUniform1i(tex1ID, 0);
	glUniform1i(tex2ID, 1);
	glUniform1i(tex3ID, 2);
	glUniform1i(tex4ID, 3);
	glUniformMatrix4fv(debInvID, 1, GL_FALSE, &invPV[0][0]);

	screen_quad->enable(3);
	screen_quad_I->draw(GL_TRIANGLES);
	screen_quad->disable();

	glEnable(GL_DEPTH_TEST);
}
