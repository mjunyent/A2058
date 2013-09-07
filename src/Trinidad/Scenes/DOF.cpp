#include "DOF.h"

DOFScene::DOFScene(Camera *cam) {
	this->cam = cam;

	quad = new VBO(director::quad, sizeof(director::quad), 0);
	quad_I = new IBO(director::quad_I, sizeof(director::quad_I));

	bool lecalite[] = { true };
	left  = new FBO(cam->width, cam->height, false, 1, lecalite);
	right = new FBO(cam->width, cam->height, false, 1, lecalite);
	mid   = new FBO(cam->width, cam->height, false, 1, lecalite);

	DOFShad    = new Shader("Shaders/Deferred/DOF.vert",    "Shaders/Deferred/DOF.frag");
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
}

void DOFScene::draw(int s, double time) {
	if(inputBuffL != NULL) {
		currentIn = inputBuffL;
		currentOut = left;
		render(s, time);
		outputBuffL = left;
	}
	if(inputBuffR != NULL) {
		currentIn = inputBuffR;
		currentOut = right;
		render(s, time);
		outputBuffR = right;
	}
}

void DOFScene::render(int s, double time) {
	glDisable(GL_DEPTH_TEST);

	//HORIZONTAL PASS
	mid->bind();
	
	DOFShad->use();

	currentIn->bind_texture(0, 0);
	currentIn->bind_depth_texture(1);

	glUniform1i(DOFTextID, 0);
	glUniform1i(DOFDepthID, 1);
	glUniform1f(DOFwidthID, cam->width);
	glUniform1f(DOFheightID, cam->height);
	glUniform1f(DOFFDistID, cam->focusDistance);

	float ms = cam->focalLength / (cam->focusDistance - cam->focalLength);
	if(ms < 0.0) {
		ms *= -1.0f;
		TOBAGO::log.write(WARNING) << "DOF Scene: Focal length is bigger than Focus Distance!";
	}

	float coeff = director::ppm * (cam->focalLength * ms) / cam->FStop;

	glUniform1f(DOFBlurCoeffID, coeff);
	
	glUniform1f(DOFFarID, cam->zfar);
	glUniform1f(DOFNearID, cam->znear);
	glUniform1i(DOFOrientationID, 0);

	quad->enable(3);
	quad_I->draw(GL_TRIANGLES);
	quad->disable();

	mid->unbind();

	//VERTICAL PASS

	currentOut->bind();

	mid->bind_texture(0, 0);
	glUniform1i(DOFOrientationID, 1);

	quad->enable(3);
	quad_I->draw(GL_TRIANGLES);
	quad->disable();

	currentOut->unbind();

	glEnable(GL_DEPTH_TEST);
}
