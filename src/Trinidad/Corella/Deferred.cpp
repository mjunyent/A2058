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
	AAShad  = new Shader("Shaders/Deferred/third.vert",  "Shaders/Deferred/third.frag");
	DOFShad = new Shader("Shaders/Deferred/DOF.vert", "Shaders/Deferred/DOF.frag");
	debugShad  = new Shader("Shaders/Deferred/debug.vert",  "Shaders/Deferred/debug.frag");

	//Prepare FBOs
	bool calite[] = { true, true, true };
	renderBuffer = new FBO(cam->width, cam->height, true, 3, calite);
	bool lecalite[] = { true };
	SecondRenderBuff = new FBO(cam->width, cam->height, false, 1, lecalite);
	DOFRenderBuff = NULL;
	AARenderBuff  = NULL;

	//By default:
	doAA = true;
	doDOF = false;
	doOffscreen = false;

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
	finalNID = AAShad->getUniform("Normal");
	finalDID = AAShad->getUniform("Depth");
	finalID  = AAShad->getUniform("Final");
	widthID  = AAShad->getUniform("width");
	heightID = AAShad->getUniform("height");


	//DOF Shader IDs
	DOFTextID		 = DOFShad->getUniform("Texture");
	DOFDepthID		 = DOFShad->getUniform("Depth");
	DOFBlurCoeffID	 = DOFShad->getUniform("BlurCoeff");
	DOFFDistID		 = DOFShad->getUniform("FocalDistance");
	DOFFarID		 = DOFShad->getUniform("Far");
	DOFNearID		 = DOFShad->getUniform("Near");
	DOFOrientationID = DOFShad->getUniform("Orientation");
	DOFwidthID		 = DOFShad->getUniform("width");
	DOFheightID		 = DOFShad->getUniform("height");

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
	if(doDOF || doAA || doOffscreen) SecondRenderBuff->bind();
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
	if(doDOF || doAA || doOffscreen) SecondRenderBuff->unbind();
}

void Deferred::DOFPass() {
	if(!doDOF) return;

	glDisable(GL_DEPTH_TEST);


	//HORIZONTAL PASS
	DOFTempRenderBuff->bind();
	
	DOFShad->use();
	
	SecondRenderBuff->bind_texture(0, 0);
	renderBuffer->bind_depth_texture(1);

	glUniform1i(DOFTextID, 0);
	glUniform1i(DOFDepthID, 1);
	glUniform1f(DOFwidthID, cam->width);
	glUniform1f(DOFheightID, cam->height);
	glUniform1f(DOFFDistID, cam->focusDistance);

	float ms = cam->focalLength / (cam->focusDistance - cam->focalLength);
	if(ms < 0.0) {
		ms *= -1.0f;
		TOBAGO::log.write(WARNING) << "Corella Engine: Focal length is bigger than Focus Distance!";
	}

	float coeff = director::ppm * (cam->focalLength * ms) / cam->FStop;

	glUniform1f(DOFBlurCoeffID, coeff);
	
	glUniform1f(DOFFarID, cam->zfar);
	glUniform1f(DOFNearID, cam->znear);
	glUniform1i(DOFOrientationID, 0);

	screen_quad->enable(3);
	screen_quad_I->draw(GL_TRIANGLES);
	screen_quad->disable();


	//VERTICAL PASS
	if(doAA || doOffscreen) DOFRenderBuff->bind();

	DOFTempRenderBuff->bind_texture(0, 0);
	glUniform1i(DOFOrientationID, 1);

	screen_quad->enable(3);
	screen_quad_I->draw(GL_TRIANGLES);
	screen_quad->disable();

	if(doAA || doOffscreen) DOFRenderBuff->unbind();

	glEnable(GL_DEPTH_TEST);

}

void Deferred::AAPass() {
	if(!doAA) return;
	if(doOffscreen) AARenderBuff->bind();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	AAShad->use();

	if(doDOF) DOFRenderBuff->bind_texture(0, 0);
	else SecondRenderBuff->bind_texture(0, 0);

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
	if(doOffscreen) AARenderBuff->unbind();
}

void Deferred::draw(int s, double t) {
	PreFirstPass();
	render(s, t);
	PostFirstPass();

	if(s == debScreen) { 
		Debug();
		return;
	}

	SecondPass();
	DOFPass();
	AAPass();
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

void Deferred::dotheAA(bool doit) {
	doAA = doit;
}

void Deferred::dotheDOF(bool doit) {
	doDOF = doit;
	bool lecalite[] = { true };
	if(DOFRenderBuff == NULL) {
		DOFTempRenderBuff = new FBO(cam->width, cam->height, false, 1, lecalite);
		DOFRenderBuff	  = new FBO(cam->width, cam->height, false, 1, lecalite);
	}
}

void Deferred::renderOfscreen(bool doit) {
	doOffscreen = doit;
	if(AARenderBuff == NULL) {
		bool lecalite[] = { true };
		AARenderBuff	 = new FBO(cam->width, cam->height, false, 1, lecalite);
	}
}