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
	left = new FBO(csp->data.width, csp->data.height, true, 1, lecalite);
	right = new FBO(csp->data.width, csp->data.height, true, 1, lecalite);
	
	ballTex = TBO("Images/Balls/noMM/BallM150.fw.png", true);
	renderedCellTemp = new FBO(ballTex.width, ballTex.height, false, 1, lecalite);
	renderedCell     = new FBO(ballTex.width, ballTex.height, false, 1, lecalite);

	billboardShad = new Shader("Shaders/Vladivostok/storm.vert", "Shaders/Vladivostok/storm.geom", "Shaders/Vladivostok/storm.frag");
	billboard_M_Id		 = billboardShad->getUniform("Model");
	billboard_V_Id		 = billboardShad->getUniform("View");
	billboard_P_Id		 = billboardShad->getUniform("Projection");
	billboard_up_id		 = billboardShad->getUniform("up");
	billboard_camPos_Id  = billboardShad->getUniform("camPos");
	billboard_r_Id		 = billboardShad->getUniform("r");
	billboard_tex_Id	 = billboardShad->getUniform("tex");
	billboard_texSize_Id = billboardShad->getUniform("texSize");
	billboard_depth_Id	 = billboardShad->getUniform("depth");

	blur = new Shader("Shaders/Post/general.vert", "Shaders/Vladivostok/stormBlur.frag");
	blur_tex_Id				= blur->getUniform("Texture");
	blur_texelSize_Id		= blur->getUniform("TexelSize");
	blur_orientation_Id		= blur->getUniform("Orientation");
	blur_amount_Id			= blur->getUniform("BlurAmount");
	blur_strength_Id		= blur->getUniform("BlurStrength");

	c = new Cells(20, 
		csp->getf("Cells.Velocity"),
		csp->getf("Cells.xRange"),
		csp->getf("Cells.yRange"),
		csp->getf("Cells.zNear"),
		csp->getf("Cells.zFar"),
		csp->getf("Cells.zFarAway"),
		csp->getf("Cells.K"),
		csp->getf("Cells.L"),
		csp->getf("Cells.M"));

	s = new Scanner(vec3(csp->getf("Scan.box.left"), csp->getf("Scan.box.up"), csp->getf("Scan.box.near")),
					vec3(csp->getf("Scan.box.right"), csp->getf("Scan.box.down"), csp->getf("Scan.box.far")),
					c);
	s->debSetup();
}

//OPTIMIZA: TODO, un solo render para las dos camaras, de las partículas!!
void Storm::draw(int s, double t) {
	//Mono
/*	currentV = &myCam->V;
	currentCamPos = &myCam->position;
	render(s, t);*/
	mat4 idd = translate(0.0f, 0.0f, 0.0f);

	//Stereo
	currentV = &myRig->V_left;
	currentCamPos = &myRig->positionL;
	current = left;
	current->bind();
	current->unbind();

//	glClearColor(0.0, 4.0/255.0, 18.0/255.0, 1.0);
	render(s, t);
	current->bind(false);
	this->s->renderDebugBox(&idd, currentV, &myCam->P);
	current->unbind();

	currentV = &myRig->V_right;
	currentCamPos = &myRig->positionR;
	current = right;
	current->bind();
	current->unbind();
//	glClearColor(0.0, 4.0/255.0, 18.0/255.0, 1.0);
	render(s, t);
	current->bind(false);
	this->s->renderDebugBox(&idd, currentV, &myCam->P);
	current->unbind();

	outputBuffL = left;
	outputBuffR = right;

}

void Storm::render(int s, double t) {
	for(int i=0; i<c->cells.size(); i++) {
		int radius = 0;
		float dist = length(myCam->position - c->cells[i].p);

		if(dist < COCNear) {
			radius = (int) -1*blurNearMax / (COCNear - myCam->znear) * dist + (blurNearMax*COCNear)/(COCNear - myCam->znear);
		} else if(dist > COCFar) {
			radius = (int) blurFarMax / (myCam->zfar - COCFar) * dist - (blurFarMax*COCFar)/(myCam->zfar - COCFar);
		}

		if(radius > 1) {
			renderCell(radius);
		}

		TOBAGO::log.write(DEBUG) << "Radius is: " << radius;

		current->bind(false);
		billboardShad->use();

//		ballTex.bind(0);
		if(radius > 1)	renderedCell->bind_texture(0, 0);
		else ballTex.bind(0);
		glUniformMatrix4fv(billboard_V_Id, 1, GL_FALSE, &(*currentV)[0][0]);
		glUniformMatrix4fv(billboard_P_Id, 1, GL_FALSE, &myCam->P[0][0]);
		glUniform3fv(billboard_up_id, 1, &myCam->up[0]);
		glUniform3fv(billboard_camPos_Id, 1, &(*currentCamPos)[0]);
		glUniform1f(billboard_r_Id, quadSize);
		glUniform1i(billboard_tex_Id, 0);
		glUniform1f(billboard_texSize_Id, texSize);

		mat4 idd = translate(c->cells[i].p);
		glUniformMatrix4fv(billboard_M_Id, 1, GL_FALSE, &idd[0][0]); 
		glUniform1f(billboard_depth_Id, fabs(c->cells[i].p.z/c->zFar));

		singlePoint->enable(3);
		singlePoint->draw(GL_POINTS);
		singlePoint->disable();
		current->unbind();
	}
}

void Storm::update(double t) {
	readConf();

	c->update();
	s->detect();
}

void Storm::readConf() {
	csp->parse();
	csp->passToCam(myCam);
	csp->passToRig(myRig);

	quadSize = csp->getf("Storm.Size");
	texSize = csp->getf("Storm.texSize");
	blurStrength = csp->getf("Storm.blurStrength");

	COCNear = csp->getf("Storm.DOF.COCnear");
	COCFar  = csp->getf("Storm.DOF.COCfar");
	blurNearMax = csp->getf("Storm.DOF.radiusNear");
	blurFarMax  = csp->getf("Storm.DOF.radiusFar");

	c->readConf(csp);
	s->readConf(csp);
}

void Storm::renderCell(int radius) {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	renderedCellTemp->bind();
		blur->use();
		ballTex.bind(0);
		glUniform1i(blur_tex_Id, 0);
		glUniform1i(blur_orientation_Id, 0);
		glUniform1i(blur_amount_Id, radius);
		glUniform1f(blur_strength_Id, blurStrength);
		glUniform2f(blur_texelSize_Id, 1.0/float(ballTex.width), 1.0/float(ballTex.height));
		
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
}
