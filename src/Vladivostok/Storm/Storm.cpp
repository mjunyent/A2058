#include "Storm.h"

Storm::Storm(CSParser *csp) {
	this->csp = csp;

	myCam = new Camera(csp->data.width, csp->data.height,
					   csp->data.FOV, csp->data.zNear, csp->data.zFar,
					   csp->data.CamPos, csp->data.CamDir, csp->data.CamUp,
					   csp->data.FocalLength, csp->data.FocusDistance, csp->data.FStop);

	myRig = new Rig(*myCam, csp->data.EyeSep);

	GLfloat zerozerozero[] = { 0.0, 0.0, 0.0 };
	singlePoint = new VBO(zerozerozero, sizeof(GLfloat)*3, 0);

	quad = new VBO(director::quad, sizeof(director::quad), 0);
	quad_I = new IBO(director::quad_I, sizeof(director::quad_I));
	
	bool lecalite[] = { true };
	bool lecaliteD[] = { true, true };
	left = new FBO(csp->data.width, csp->data.height, true, 2, lecaliteD);
	right = new FBO(csp->data.width, csp->data.height, true, 2, lecaliteD);
	
	ballTex.push_back(new TBO("Images/Balls/6P/1.png", true));
	ballTex.push_back(new TBO("Images/Balls/6P/2.png", true));
	ballTex.push_back(new TBO("Images/Balls/6P/3.png", true));
	ballTex.push_back(new TBO("Images/Balls/6P/4.png", true));
	ballTex.push_back(new TBO("Images/Balls/6P/5.png", true));
	ballTex.push_back(new TBO("Images/Balls/6P/6.png", true));
	ballTex[0]->clamp(true);
	renderedCellTemp = new FBO(ballTex[0]->width, ballTex[0]->height, false, 1, lecalite);
	renderedCell     = new FBO(ballTex[0]->width, ballTex[0]->height, false, 1, lecalite);

	billboardShad = new Shader("Shaders/Vladivostok/storm.vert", "Shaders/Vladivostok/storm.geom", "Shaders/Vladivostok/storm.frag");
	billboard_M_Id		 = billboardShad->getUniform("Model");
	billboard_V_Id		 = billboardShad->getUniform("View");
	billboard_P_Id		 = billboardShad->getUniform("Projection");
	billboard_up_id		 = billboardShad->getUniform("up");
	billboard_camPos_Id  = billboardShad->getUniform("camPos");
	billboard_r_Id		 = billboardShad->getUniform("r");
	billboard_tex_Id	 = billboardShad->getUniform("tex");
	billboard_cellPos_Id = billboardShad->getUniform("cellPos");
	billboard_alpha_Id	 = billboardShad->getUniform("alpha");
	billboard_side_Id    = billboardShad->getUniform("side");

	blur = new Shader("Shaders/Post/general.vert", "Shaders/Vladivostok/stormBlur.frag");
	blur_tex_Id				= blur->getUniform("Texture");
	blur_texelSize_Id		= blur->getUniform("TexelSize");
	blur_orientation_Id		= blur->getUniform("Orientation");
	blur_amount_Id			= blur->getUniform("BlurAmount");
	blur_strength_Id		= blur->getUniform("BlurStrength");

	c = new Cells(80, csp);

	s = new Scanner(csp, c, myRig);
	s->debSetup();

	lastRadius = 0;
	readConf();
}

void Storm::draw(int s, double t) {
	mat4 idd = translate(0.0f, 0.0f, 0.0f);

	this->s->renderModel();

	left->bind();
//	this->s->renderDebugBox(&idd, &myRig->V_left, &myCam->P);
	left->unbind();
	right->bind();
//	this->s->renderDebugBox(&idd, &myRig->V_right, &myCam->P);
	right->unbind();

	render(s, t);

	outputBuffL = left;
	outputBuffR = right;
}

void Storm::render(int s, double t) {
	for(int i=0; i<c->sortedCells.size(); i++) {
		if(c->sortedCells[i]->id != this->s->scanningCell || this->s->status == STATE::START)  renderCell(c->sortedCells[i]->id, 2, 2, 0);
		else {
			float leftPos = this->s->draw(&myRig->V_left, &myCam->P, left, true);
			float rightPos = this->s->draw(&myRig->V_right, &myCam->P, right, false);
			renderCell(this->s->scanningCell, leftPos, rightPos, this->s->side);
		}
	}
}

void Storm::update(double t) {
//	readConf();

	c->update();
	s->update();
	c->sortThing();

//	s->detect();
}

void Storm::readConf() {
	csp->parse();
	csp->passToCam(myCam);
	csp->passToRig(myRig);

	quadSize = csp->getf("Storm.Size");
	blurStrength = csp->getf("Storm.blurStrength");

	COCNear = csp->getf("Storm.DOF.COCnear");
	COCFar  = csp->getf("Storm.DOF.COCfar");
	blurNearMax = csp->getf("Storm.DOF.radiusNear");
	blurFarMax  = csp->getf("Storm.DOF.radiusFar");

	c->readConf(csp);
	s->readConf(csp);
}

void Storm::renderCell(int i, float cellScreenPositionL, float cellScreenPositionR, int side) {
	int radius = 0;
	float dist = length(myCam->position - c->cells[i].p);
	mat4 idd = translate(c->cells[i].p);

	vec4 screenP = myCam->P * myCam->V * idd * vec4(0,0,0,1);
	screenP /= screenP.w;
	if(!inRange(screenP.x, -1.1f, 1.1f) ||
	   !inRange(screenP.y, -1.1f, 1.1f) ||
	   !inRange(screenP.z, -1.1f, 1.1f)) {
		   return;
	}

	if(dist < COCNear) {
		radius = (int) -1*blurNearMax / (COCNear - myCam->znear) * dist + (blurNearMax*COCNear)/(COCNear - myCam->znear);
	} else if(dist > COCFar) {
		radius = (int) blurFarMax / (myCam->zfar - COCFar) * dist - (blurFarMax*COCFar)/(myCam->zfar - COCFar);
	}
	radius+=2;

//	if(lastRadius != radius) {
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	
		renderedCellTemp->bind();
			blur->use();
			ballTex[c->cells[i].color]->bind(0);
			glUniform1i(blur_tex_Id, 0);
			glUniform1i(blur_orientation_Id, 0);
			glUniform1i(blur_amount_Id, radius);
			glUniform1f(blur_strength_Id, blurStrength);
			glUniform2f(blur_texelSize_Id, 1.0/float(ballTex[c->cells[i].color]->width), 1.0/float(ballTex[c->cells[i].color]->height));
		
			quad->enable(3);
			quad_I->draw(GL_TRIANGLES);
			quad->disable();
		renderedCellTemp->unbind();
	
		renderedCell->bind();
			renderedCellTemp->bind_texture(0, 0);
			glUniform1i(blur_tex_Id, 0);
			glUniform1i(blur_orientation_Id, 1);

			quad->enable(3);
			quad_I->draw(GL_TRIANGLES);
			quad->disable();
		renderedCell->unbind();
	
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
//	}

//	lastRadius = radius;

	//Left
	left->bind(false);
	billboardShad->use();

	renderedCell->bind_texture(0, 0);
	glUniformMatrix4fv(billboard_M_Id, 1, GL_FALSE, &idd[0][0]); 
	glUniformMatrix4fv(billboard_V_Id, 1, GL_FALSE, &myRig->V_left[0][0]);
	glUniformMatrix4fv(billboard_P_Id, 1, GL_FALSE, &myCam->P[0][0]);
	glUniform3fv(billboard_up_id, 1, &myCam->up[0]);
	glUniform3fv(billboard_camPos_Id, 1, &myRig->positionL[0]);
	glUniform1f(billboard_r_Id, quadSize);
	glUniform1i(billboard_tex_Id, 0);
	glUniform1f(billboard_cellPos_Id, (cellScreenPositionL+1.02)/2.0*float(myCam->width));
	glUniform1i(billboard_side_Id, side);
	glUniform1f(billboard_alpha_Id, c->cells[i].alpha);

	singlePoint->enable(3);
	singlePoint->draw(GL_POINTS);
	singlePoint->disable();
	left->unbind();

	//Right
	right->bind(false);
	billboardShad->use();

	renderedCell->bind_texture(0, 0);
	glUniformMatrix4fv(billboard_M_Id, 1, GL_FALSE, &idd[0][0]); 
	glUniformMatrix4fv(billboard_V_Id, 1, GL_FALSE, &myRig->V_right[0][0]);
	glUniformMatrix4fv(billboard_P_Id, 1, GL_FALSE, &myCam->P[0][0]);
	glUniform3fv(billboard_up_id, 1, &myCam->up[0]);
	glUniform3fv(billboard_camPos_Id, 1, &myRig->positionR[0]);
	glUniform1f(billboard_r_Id, quadSize);
	glUniform1i(billboard_tex_Id, 0);
	glUniform1f(billboard_cellPos_Id, (cellScreenPositionR+1.02)/2.0*float(myCam->width));
	glUniform1f(billboard_alpha_Id, c->cells[i].alpha);

	singlePoint->enable(3);
	singlePoint->draw(GL_POINTS);
	singlePoint->disable();
	right->unbind();
}
