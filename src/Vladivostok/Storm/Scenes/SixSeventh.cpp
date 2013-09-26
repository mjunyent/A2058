#include "SixSeventh.h"
#include "../Scanner.h"

SixthRendererSkull::SixthRendererSkull(CSParser *csp, Camera *cam) : Deferred() {
	this->csp = csp;

	setup(cam);
	
	Skull_3DS = new A3dsHandler("Models/Storm/6Skull.3DS", 0);
	Skull_3DS->readNormalsFromFile("Models/Storm/6SkullNormals0.txt");
	Skull_3DS->makeUVs();
	Skull_3DS->makeBoundingBox();

	Skull = new Model(firstShad,
					  Skull_3DS->vertexs,
					  Skull_3DS->normals,
					  Skull_3DS->UVs,
					  NULL,
					  NULL,
					  Skull_3DS->indexs,
					  0.6,
					  vec3(120.0f/255.0f, 105.0f/255.0f, 80.0f/255.0f),
					  vec3(0.4f, 0.4f, 0.4f),
					  0.1f,
					  &Skull_M,
					  SkullSize/Skull_3DS->maxDimension,
					  "Images/Textures/texture_skull.png",
					  NULL);

	Jaw_3DS = new A3dsHandler("Models/Storm/6Skull.3DS", 1);
	Jaw_3DS->readNormalsFromFile("Models/Storm/6SkullNormals1.txt");
	Jaw_3DS->makeUVs();
	Jaw_3DS->makeBoundingBox();

	Jaw = new Model(firstShad,
					Jaw_3DS->vertexs,
					Jaw_3DS->normals,
					Jaw_3DS->UVs,
					NULL,
					NULL,
					Jaw_3DS->indexs,
					0.6,
					vec3(120.0f/255.0f, 105.0f/255.0f, 80.0f/255.0f),
					vec3(0.4f, 0.4f, 0.4f),
					0.1f,
					&Jaw_M,
					SkullSize/Jaw_3DS->maxDimension,
					"Images/Textures/texture_skull.png",
					NULL);

	dotheAA(true);
	dotheDOF(false);
	dotheAO(2, 0.05, vec2(2, 2), true);

	readConf(csp);
}

void SixthRendererSkull::setPosition(vec3 *position) {
	pos = position;
//	rotate_M = rotate_M * rotate(-1.0f*rotationVel, 0.0f, 1.0f, 0.0f);
	
	vec3 dir = cam->position - *position;
	dir = normalize(dir);
//	Skull_M = glm::translate(zLate*dir) * glm::translate(*position) * rotate_M * rotate(-90.0f, 1.0f, 0.0f, 0.0f) * glm::translate(-Brain_3DS->center*Brain->scale);

	vec3 skullTranslate(0.0f, 0.880123f, 3.0925f);
//	skullTranslate *= Skull->scale;

	vec3 jawTranslate(0.0, -0.26, 4.25893);
//	jawTranslate *= Skull->scale;

	mat4 general = glm::translate(zLate*dir) * glm::translate(*position) * rotate(-90.0f, 1.0f, 0.0f, 0.0f)
		    * glm::translate(-Skull_3DS->center*Skull->scale);

	Skull_M = general*glm::translate(-skullTranslate);

	Jaw_M = general * glm::translate(-jawTranslate);
}

void SixthRendererSkull::render(int s, double t) {
	Skull->render();
	Jaw->render();
}

void SixthRendererSkull::readConf(CSParser *csp) {
	zLate = csp->getf("Scenes.SixSeventh.Skull.zLate");
	SkullSize = csp->getf("Scenes.SixSeventh.Skull.size");
	Skull->scale = SkullSize/Skull_3DS->maxDimension;
	Jaw->scale = Skull->scale;
	AO_radius = csp->getf("Scenes.SixSeventh.Skull.AO.radius");
	AO_bias   = csp->getf("Scenes.SixSeventh.Skull.AO.bias");
	AO_attenuation = vec2(csp->getf("Scenes.SixSeventh.Skull.AO.linearAtt"),
						  csp->getf("Scenes.SixSeventh.Skull.AO.quadraticAtt"));

	csp->readLights("Scenes.SixSeventh.Skull.Lights");
	csp->passToLight(lights);
}

void SixthRendererSkull::update(double t) {
	readConf(csp);
	setPosition(pos);
}








SeventhRendererBrain::SeventhRendererBrain(CSParser *csp, Camera *cam) : Deferred() {
	this->csp = csp;
//	secondShad = new Shader("Shaders/Deferred/second.vert", "Shaders/Vladivostok/Scenes/Polio.frag");

	setup(cam);
	
	Brain_3DS = new A3dsHandler("Models/Storm/7Brain.3DS", 0);
	Brain_3DS->readNormalsFromFile("Models/Storm/7BrainNormals.txt");
//	Polio_3DS->makeNormalsPerVertex();
	Brain_3DS->makeBoundingBox();

	Brain = new Model(firstShad,
					  Brain_3DS->vertexs,
					  Brain_3DS->normals,
					  NULL,
					  NULL,
					  NULL,
					  Brain_3DS->indexs,
					  0.4,
					  vec3(120.0f/255.0f, 105.0f/255.0f, 80.0f/255.0f),
					  vec3(1.0f, 1.0f, 1.0f),
					  0.51f,
					  &Brain_M,
					  BrainSize/Brain_3DS->maxDimension,
					  NULL,
					  NULL);

	dotheAA(true);
	dotheDOF(false);
	dotheAO(2, 0.05, vec2(2, 2), true);

	readConf(csp);
}

void SeventhRendererBrain::setPosition(vec3 *position) {
	pos = position;
	rotate_M = rotate_M * rotate(-1.0f*rotationVel, 0.0f, 1.0f, 0.0f);
	
	vec3 dir = cam->position - *position;
	dir = normalize(dir);
	Brain_M = glm::translate(zLate*dir) * glm::translate(*position) * rotate_M * rotate(-90.0f, 1.0f, 0.0f, 0.0f) * glm::translate(-Brain_3DS->center*Brain->scale);
}

void SeventhRendererBrain::render(int s, double t) {
	Brain->render();
}

void SeventhRendererBrain::readConf(CSParser *csp) {
	zLate = csp->getf("Scenes.SixSeventh.Brain.zLate");
	BrainSize = csp->getf("Scenes.SixSeventh.Brain.size");
	Brain->scale = BrainSize/Brain_3DS->maxDimension;
	AO_radius = csp->getf("Scenes.SixSeventh.Brain.AO.radius");
	AO_bias   = csp->getf("Scenes.SixSeventh.Brain.AO.bias");
	AO_attenuation = vec2(csp->getf("Scenes.SixSeventh.Brain.AO.linearAtt"),
						  csp->getf("Scenes.SixSeventh.Brain.AO.quadraticAtt"));
	rotationVel = csp->getf("Scenes.SixSeventh.Brain.rotationVel");

	csp->readLights("Scenes.SixSeventh.Brain.Lights");
	csp->passToLight(lights);
}

void SeventhRendererBrain::update(double t) {
	readConf(csp);
	setPosition(pos);
}




SixSeventhStormScene::SixSeventhStormScene(CSParser *csp, Scanner *s) : StormScene(s) {
	readConf(csp);

//	renderFw = new FirstRendererWorld(csp, scan->rig);
//	renderFp = new FirstRenderPolio(csp, scan->rig);

	firstStill = true;

	skullText = TBO("Images/Biotechnopolis/060R.fw.png", true);
	brainText = TBO("Images/Biotechnopolis/070R.fw.png", true);
	skullText.clamp(true);
	brainText.clamp(true);

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

	ratio = float(skullText.width)/float(skullText.height);
	float textWidth = ratio*textHeight;

	float rectText[] = {
		 linesLeftEnd-textWidth,  textHeight/2.0f, 0.0f, //0 UP, LEFT
		 linesLeftEnd,			  textHeight/2.0f, 0.0f, //1 UP, RIGHT
		 linesLeftEnd,			 -textHeight/2.0f, 0.0f, //2 DOWN, RIGHT
		 linesLeftEnd-textWidth, -textHeight/2.0f, 0.0f  //3 DOWN, LEFT
	};

	textQuad = new VBO(rectText, sizeof(director::quad), 0);
}

void SixSeventhStormScene::renderModel() {
//	renderFw->draw(0, 0);
//	if(scan->status == STATE::UNSCAN || !firstStill) renderFp->draw(0, 0);
}

void SixSeventhStormScene::modelDraw(mat4 *V, mat4 *P, FBO *render, bool left) {
/*	render->bind(false);
	glDisable(GL_DEPTH_TEST);
	scan->mixShad->use();
	if(left) {
		renderFw->outputBuffL->bind_texture(0, 0);
		renderFw->renderBufferL->bind_depth_texture(1);
	}
	else {
		renderFw->outputBuffR->bind_texture(0, 0);
		renderFw->renderBufferR->bind_depth_texture(1);
	}
	if(scan->status == STATE::UNSCAN || !firstStill) {
		if(left) {
			renderFp->outputBuffL->bind_texture(0, 2);
			renderFp->renderBufferL->bind_depth_texture(3);
		}
		else {
			renderFp->outputBuffR->bind_texture(0, 2);
			renderFp->renderBufferR->bind_depth_texture(3);
		}
	}

	if(scan->status == STATE::UNSCAN || !firstStill) glUniform1i(scan->mix_showL_Id, 1);
	else glUniform1i(scan->mix_showL_Id, 0);

	if(scan->status == STATE::GRID && !firstStill) glUniform1i(scan->mix_showR_Id, 0);
	else glUniform1i(scan->mix_showR_Id, 1);

	glUniform1i(scan->mix_TexR_Id, 0);
	glUniform1i(scan->mix_DepthR_Id, 1);
	glUniform1i(scan->mix_TexL_Id, 2);
	glUniform1i(scan->mix_DepthL_Id, 3);
	glUniform1f(scan->mix_position_Id, scan->worldToClip(V, P, &scan->gridPositionVec));
	scan->quad->enable(3);
	scan->quad_I->draw(GL_TRIANGLES);
	scan->quad->disable();
	glEnable(GL_DEPTH_TEST);
	render->unbind();*/
}

void SixSeventhStormScene::linesDraw(mat4 *V, mat4 *P, FBO *render) {
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
	if(scan->status == STATE::UNSCAN || !firstStill) glUniform1i(scan->text_showL_Id, 1);
	else glUniform1i(scan->text_showL_Id, 0);
	if(scan->status == STATE::GRID && !firstStill) glUniform1i(scan->text_showR_Id, 0);
	else glUniform1i(scan->text_showR_Id, 1);
	
	linesQuad->enable(3);
	scan->textQuadCoords->enable(3);
	scan->quad_I->draw(GL_TRIANGLES);
	scan->textQuadCoords->disable();
	linesQuad->disable();
	render->unbind();


}

void SixSeventhStormScene::textDraw(mat4 *V, mat4 *P, FBO *render) {
	mat4 M = translate(scan->cells->cells[scan->scanningCell].p);

	render->bind(false);
	scan->textShad->use();
	skullText.bind(0);
	if(scan->status == STATE::UNSCAN || !firstStill) brainText.bind(1);
	glUniformMatrix4fv(scan->text_M_Id, 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv(scan->text_V_Id, 1, GL_FALSE, &(*V)[0][0]); 
	glUniformMatrix4fv(scan->text_P_Id, 1, GL_FALSE, &(*P)[0][0]);
	glUniform1f(scan->text_sP_Id, (scan->worldToClip(V, P, &scan->gridPositionVec)+1.0f)/2.0f*float(render->width));
	glUniform1i(scan->text_rightTex_Id, 0);
	glUniform1i(scan->text_leftTex_Id, 1);

	if(scan->status == STATE::GRID && !firstStill) glUniform1i(scan->text_showR_Id, 0);
	else glUniform1i(scan->text_showR_Id, 1);

	if(scan->status == STATE::UNSCAN || !firstStill) glUniform1i(scan->text_showL_Id, 1);
	else glUniform1i(scan->text_showL_Id, 0);
	
	textQuad->enable(3);
	scan->textQuadCoords->enable(3);
	scan->quad_I->draw(GL_TRIANGLES);
	scan->textQuadCoords->disable();
	textQuad->disable();
	render->unbind();
}

void SixSeventhStormScene::update() {
//	renderFw->setPosition(&scan->cells->cells[scan->scanningCell].p);
//	renderFp->setPosition(&scan->cells->cells[scan->scanningCell].p);
}

void SixSeventhStormScene::readConf(CSParser *csp) {
	textHeight	= csp->getf("Scenes.SixSeventh.textHeight");
	linesHeight	= csp->getf("Scenes.SixSeventh.linesHeight");
	stillTime	= csp->getf("Scenes.SixSeventh.stillTime");
}

STATE SixSeventhStormScene::flowControl() {
	STATE now = scan->status;

	if(now == STATE::GRID) {
		if(firstStill) return STATE::STILL;
		else {
			scan->side = 1;
			firstStill = true;
			return STATE::REST;
		}
	}

	if(now == STATE::STILL)  {
		if(firstStill) {
			firstStill = false;
			scan->side = 3;
			return STATE::UNSCAN;
		} else {
			scan->side = 2;
			return STATE::GRID;
		}
	}

	if(now == STATE::UNSCAN) return STATE::STILL;

}