#include "Eight.h"
#include "../Scanner.h"


EightRendererRBC::EightRendererRBC(CSParser *csp, Camera *cam, FBO *rL, FBO *rR) : Deferred() {
	renderBufferL = rL;
	renderBufferR = rR;
	secondShad = new Shader("Shaders/Deferred/second.vert", "Shaders/Vladivostok/Scenes/RBC.frag");

	setup(cam);
	this->csp = csp;

	RBC_3DS = new A3dsHandler("Models/Storm/8RBC.3DS", 0);
//	Flu_3DS->makeNormalsPerVertex();
	RBC_3DS->readNormalsFromFile("Models/Storm/8RBCNormals.txt");
	RBC_3DS->makeBoundingBox();

	RBC = new Model(firstShad,
					RBC_3DS->vertexs,
					RBC_3DS->normals,
					NULL,
					NULL,
					NULL,
					RBC_3DS->indexs,
					0.2,
					vec3(124.0f/255.0f, 114.0f/255.0f, 0.0f),
					vec3(0.1f, 0.1f, 0.1f),
					0.1f,
					&RBC_M,
					RBCSize/RBC_3DS->maxDimension,
					NULL,
					NULL);

	dotheAA(true);
	dotheDOF(false);
	dotheAO(2, 0.05, vec2(2, 2), true);

	readConf(csp);

	for(int i=0; i<6; i++) {
		rotVals[i] = 0.0;
	}

	setRotVals();
}

void EightRendererRBC::setPosition(vec3 *position) {
	pos = position;

	for(int i=0; i<6; i++) {
		rotVals[i] += rotValue+rotVelsAdded[i];
	}
}

void EightRendererRBC::render(int s, double t) {
	for(int i=0; i<6; i++) {
		RBC_M = translate(*pos) * translate(RBCdisp[i]) * rotate(rotVals[i], rotVecs[i]) * translate(-RBC_3DS->center*RBC->scale);
		RBC->render();
	}
}

void EightRendererRBC::update(double t) {
	readConf(csp);
	setPosition(pos);
}

void EightRendererRBC::readConf(CSParser *csp) {
	csp->parse();
	RBCSize = csp->getf("Scenes.Eight.RBC.size");
	RBC->scale = RBCSize/RBC_3DS->maxDimension;
	
	RBC->shininess = csp->getf("Scenes.Eight.RBC.shininess");
	RBC->diffuse_color = csp->getvec3("Scenes.Eight.RBC.diffuseColor");
	RBC->specular_color = csp->getvec3("Scenes.Eight.RBC.specularColor");

	AO_radius = csp->getf("Scenes.Eight.RBC.AO.radius");
	AO_bias   = csp->getf("Scenes.Eight.RBC.AO.bias");
	AO_attenuation = vec2(csp->getf("Scenes.Eight.RBC.AO.linearAtt"),
						  csp->getf("Scenes.Eight.RBC.AO.quadraticAtt"));

	csp->readLights("Scenes.Eight.RBC.Lights");
	csp->passToLight(lights);

	RBCdisp[0] = csp->getvec3("Scenes.Eight.RBC.disp0");
	RBCdisp[1] = csp->getvec3("Scenes.Eight.RBC.disp1");
	RBCdisp[2] = csp->getvec3("Scenes.Eight.RBC.disp2");
	RBCdisp[3] = csp->getvec3("Scenes.Eight.RBC.disp3");
	RBCdisp[4] = csp->getvec3("Scenes.Eight.RBC.disp4");
	RBCdisp[5] = csp->getvec3("Scenes.Eight.RBC.disp5");

	rotValue = csp->getf("Scenes.Eight.RBC.rotationVel");
}

void EightRendererRBC::setRotVals() {
	for(int i=0; i<6; i++) {
		rotVecs[i] = normalize(vec3(randValue(-1,1), randValue(-1,1), randValue(-1,1)));
		rotVelsAdded[i] = randValue(-0.1,0.1);
	}
}


EightStormScene::EightStormScene(CSParser *csp, Scanner *s, FBO *rL, FBO *rR) : StormScene(s) {
	readConf(csp);

	renderRBC = new EightRendererRBC(csp, scan->rig, rL, rR);

	heart = TBO("Images/Biotechnopolis/heart.png", true);
	text.clamp(true);

	text = TBO("Images/Biotechnopolis/080R.fw.png", true);
	text.clamp(true);
	
	heartShad = new Shader("Shaders/Vladivostok/heart.vert", "Shaders/Vladivostok/heart.frag");
	heart_M_Id = heartShad->getUniform("Model");
	heart_V_Id = heartShad->getUniform("View");
	heart_P_Id = heartShad->getUniform("Projection");
	heart_sP_Id = heartShad->getUniform("screenPosition");
	heart_Tex_Id = heartShad->getUniform("Tex");
	heart_alpha_Id = heartShad->getUniform("alpha");

	//Make texture Quads:
	float ratio = float(scan->linesCircleRight.width)/float(scan->linesCircleRight.height);
	float linesWidth = ratio*linesHeight;
	float linesLeftEnd = -linesWidth + linesHeight/2.0f;

	float linesRect[] = {
		 linesLeftEnd,		 linesHeight/2.0f, 0.0f,
		 linesHeight/2.0f,	 linesHeight/2.0f, 0.0f,
		 linesHeight/2.0f,	-linesHeight/2.0f, 0.0f,
		 linesLeftEnd,		-linesHeight/2.0f, 0.0f,
	};

	linesQuad = new VBO(linesRect, sizeof(float)*12, 0);


	ratio = float(text.width)/float(text.height);
	float textWidth = ratio*textHeight;

	float rectText[] = {
		 linesLeftEnd-textWidth,  textHeight/2.0f, 0.0f, //0 UP, LEFT
		 linesLeftEnd,			  textHeight/2.0f, 0.0f, //1 UP, RIGHT
		 linesLeftEnd,			 -textHeight/2.0f, 0.0f, //2 DOWN, RIGHT
		 linesLeftEnd-textWidth, -textHeight/2.0f, 0.0f  //3 DOWN, LEFT
	};

	textQuad = new VBO(rectText, sizeof(director::quad), 0);


	ratio = float(heart.width)/float(heart.height);
	float hearthWidth = ratio*heartHeight;

	float heartRect[] = {
		 -hearthWidth/2.0f,	 heartHeight/2.0f, 0.0f,
		  hearthWidth/2.0f,	 heartHeight/2.0f, 0.0f,
		  hearthWidth/2.0f,	-heartHeight/2.0f, 0.0f,
		 -hearthWidth/2.0f,	-heartHeight/2.0f, 0.0f,
	};

	heartQuad = new VBO(heartRect, sizeof(director::quad), 0);
	heartPosition = 0.0;
	heartVel = 0.0;
	heartAlpha = 0.0;
	heartZoom = false;

	renderRBC->setRotVals();
}

void EightStormScene::renderModel() {
	renderRBC->draw(0, 0);
}

void EightStormScene::modelDraw(mat4 *V, mat4 *P, FBO *render, bool left) {
	vec3 direction = normalize(scan->rig->position-scan->cells->cells[scan->scanningCell].p);

	mat4 M = translate(scan->cells->cells[scan->scanningCell].p + direction*(zLate + heartPosition));

	render->bind(false);
	//if(heartAlpha > 0.1) {
		glDisable(GL_DEPTH_TEST);
		scan->mixShad->use();
		if(left) {
			renderRBC->outputBuffL->bind_texture(0, 0);
			renderRBC->renderBufferL->bind_depth_texture(1);
		}
		else {
			renderRBC->outputBuffR->bind_texture(0, 0);
			renderRBC->renderBufferR->bind_depth_texture(1);
		}

		glUniform1i(scan->mix_showL_Id, 0);
		glUniform1i(scan->mix_showR_Id, 1);

		glUniform1i(scan->mix_TexR_Id, 0);
		glUniform1i(scan->mix_DepthR_Id, 1);
	
		glUniform1f(scan->mix_position_Id, scan->worldToClip(V, P, &scan->gridPositionVec));
		scan->quad->enable(3);
		scan->quad_I->draw(GL_TRIANGLES);
		scan->quad->disable();
		glEnable(GL_DEPTH_TEST);
	//}

	heartShad->use();
	heart.bind(0);
	glUniformMatrix4fv(heart_M_Id, 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv(heart_V_Id, 1, GL_FALSE, &(*V)[0][0]); 
	glUniformMatrix4fv(heart_P_Id, 1, GL_FALSE, &(*P)[0][0]);
	glUniform1f(heart_sP_Id, (scan->worldToClip(V, P, &scan->gridPositionVec)+1.0f)/2.0f*float(render->width));
	glUniform1i(heart_Tex_Id, 0);
	glUniform1f(heart_alpha_Id, heartAlpha);
	
	heartQuad->enable(3);
	scan->textQuadCoords->enable(3);
	scan->quad_I->draw(GL_TRIANGLES);
	scan->textQuadCoords->disable();
	heartQuad->disable();

	render->unbind();
}

void EightStormScene::linesDraw(mat4 *V, mat4 *P, FBO *render) {
	mat4 M = translate(scan->cells->cells[scan->scanningCell].p);

	render->bind(false);
	scan->textShad->use();
	scan->linesCircleRight.bind(0);
	glUniformMatrix4fv(scan->text_M_Id, 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv(scan->text_V_Id, 1, GL_FALSE, &(*V)[0][0]); 
	glUniformMatrix4fv(scan->text_P_Id, 1, GL_FALSE, &(*P)[0][0]);
	glUniform1f(scan->text_sP_Id, (scan->worldToClip(V, P, &scan->gridPositionVec)+1.0f)/2.0f*float(render->width));
	glUniform1i(scan->text_rightTex_Id, 0);
	glUniform1i(scan->text_leftTex_Id, 0);
	glUniform1i(scan->text_showL_Id, 0);
	glUniform1i(scan->text_showR_Id, 1);
	
	linesQuad->enable(3);
	scan->textQuadCoords->enable(3);
	scan->quad_I->draw(GL_TRIANGLES);
	scan->textQuadCoords->disable();
	linesQuad->disable();
	render->unbind();


}

void EightStormScene::textDraw(mat4 *V, mat4 *P, FBO *render) {
	mat4 M = translate(scan->cells->cells[scan->scanningCell].p);

	render->bind(false);
	scan->textShad->use();
	text.bind(0);
	glUniformMatrix4fv(scan->text_M_Id, 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv(scan->text_V_Id, 1, GL_FALSE, &(*V)[0][0]); 
	glUniformMatrix4fv(scan->text_P_Id, 1, GL_FALSE, &(*P)[0][0]);
	glUniform1f(scan->text_sP_Id, (scan->worldToClip(V, P, &scan->gridPositionVec)+1.0f)/2.0f*float(render->width));
	glUniform1i(scan->text_rightTex_Id, 0);

	glUniform1i(scan->text_showR_Id, 1);
	glUniform1i(scan->text_showL_Id, 0);
	
	textQuad->enable(3);
	scan->textQuadCoords->enable(3);
	scan->quad_I->draw(GL_TRIANGLES);
	scan->textQuadCoords->disable();
	textQuad->disable();
	render->unbind();
}

void EightStormScene::readConf(CSParser *csp) {
	textHeight	= csp->getf("Scenes.Eight.textHeight");
	linesHeight	= csp->getf("Scenes.Eight.linesHeight");
	stillTime	= csp->getf("Scenes.Eight.stillTime");
	heartHeight = csp->getf("Scenes.Eight.Heart.height");
	heartAccel	= csp->getf("Scenes.Eight.Heart.acceleration");
	heartAlphaVel = csp->getf("Scenes.Eight.Heart.alphaVel");
	zLate = csp->getf("Scenes.Eight.Heart.zLate");
}

void EightStormScene::update() {
	if(heartZoom && heartAlpha < 1.0) {
		heartVel += heartAccel; //you could integrate man...
		heartPosition += heartVel;
		heartAlpha = clamp(heartAlpha+heartAlphaVel, 0.0f, 1.0f);
	}

	renderRBC->setPosition(&scan->cells->cells[scan->scanningCell].p);
}

STATE EightStormScene::flowControl() {
	STATE now = scan->status;

	if(now == STATE::GRID) {
		heartZoom = true;
		return STATE::STILL;
	}
	if(now == STATE::STILL) return STATE::UNSCAN;
	if(now == STATE::UNSCAN) {
		heartZoom = false;
		heartPosition = 0.0;
		heartVel = 0.0;
		heartAlpha = 0.0;
		renderRBC->setRotVals();
		return STATE::REST;
	}
}