#include "Fourth.h"
#include "../Scanner.h"

FourthRendererFish::FourthRendererFish(CSParser *csp, Camera *cam, FBO *rL, FBO *rR) : Deferred() {
	renderBufferL = rL;
	renderBufferR = rR;

	setup(cam);
	this->csp = csp;

	Fish_3DS = new A3dsHandler("Models/Storm/4Krilatka.3DS", 1);
	Fish_3DS->readNormalsFromFile("Models/Storm/4KrilatkaNormals.txt");
	Fish_3DS->makeUVs();
	Fish_3DS->makeBoundingBox();

	Fish = new Model(firstShad,
					  Fish_3DS->vertexs,
					  Fish_3DS->normals,
					  Fish_3DS->UVs,
					  NULL,
					  NULL,
					  Fish_3DS->indexs,
					  0.4,
					  vec3(124.0f/255.0f, 114.0f/255.0f, 0.0f),
					  vec3(0.1f, 0.1f, 0.1f),
					  0.01f,
					  &Fish_M,
					  FishSize/Fish_3DS->maxDimension,
					  "Images/Textures/krilatka.png",
					  NULL);

	Eye_3DS = new A3dsHandler("Models/Storm/4Krilatka.3DS", 0);
	Eye_3DS->makeNormalsPerVertex();
	Eye_3DS->makeUVs();
	Eye_3DS->makeBoundingBox();

	Eye = new Model(firstShad,
					Eye_3DS->vertexs,
					Eye_3DS->normals,
					Eye_3DS->UVs,
					NULL,
					NULL,
					Eye_3DS->indexs,
					0.4,
					vec3(124.0f/255.0f, 114.0f/255.0f, 0.0f),
					vec3(0.1f, 0.1f, 0.1f),
					0.01f,
					&Eye_M,
					EyeSize/Eye_3DS->maxDimension,
					"Images/Textures/krilatkaEye.png",
					NULL);
	dotheAA(true);
	dotheDOF(false);
	dotheAO(2, 0.05, vec2(2, 2), true);

	readConf(csp);

	rotateAlpha = 0;
	//lights->addDirectionalLight(glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
}

void FourthRendererFish::setPosition(vec3 *position) {
	pos = position;

	rotateAlpha -= rotateVel;

	glm::mat4 rot = glm::rotate(rotateV.z, 0.0f, 0.0f, 1.0f) *
					glm::rotate(rotateV.x, 1.0f, 0.0f, 0.0f) *
					glm::rotate(rotateV.y, 0.0f, 1.0f, 0.0f);
	glm::mat4 tr = glm::translate(transV);

	Fish_M = tr * glm::translate(*position) * glm::rotate(rotateAlpha, 0.0f, 1.0f, 0.0f) *
		     rotate(-90.0f, 1.0f, 0.0f, 0.0f) * glm::translate(-Fish_3DS->center*Fish->scale) * rot;
	Eye_M  = tr * glm::translate(*position) * glm::rotate(rotateAlpha, 0.0f, 1.0f, 0.0f) * glm::translate(eyePos*Fish->scale) * rotate(180.0f, 0.0f, 1.0f, 0.0f) *
		rotate(-90.0f, 1.0f, 0.0f, 0.0f) * rot * glm::translate(-Eye_3DS->center*Eye->scale);
}

void FourthRendererFish::render(int s, double t) {
	Fish->render();
	Eye->render();
}

void FourthRendererFish::update(double t) {
	readConf(csp);
	setPosition(pos);
}

void FourthRendererFish::readConf(CSParser *csp) {
	csp->parse();
	FishSize = csp->getf("Scenes.Fourth.Fish.size");
	EyeSize  = csp->getf("Scenes.Fourth.Eye.size");
	eyePos = csp->getvec3("Scenes.Fourth.Eye.position");
	rotateV = csp->getvec3("Scenes.Fourth.Fish.rotate");
	transV = csp->getvec3("Scenes.Fourth.translate");
	if(Fish != NULL) Fish->scale = FishSize/Fish_3DS->maxDimension;
	if(Eye  != NULL) Eye->scale  = EyeSize/Eye_3DS->maxDimension;

	AO_radius = csp->getf("Scenes.Fourth.Fish.AO.radius");
	AO_bias   = csp->getf("Scenes.Fourth.Fish.AO.bias");
	AO_attenuation = vec2(csp->getf("Scenes.Fourth.Fish.AO.linearAtt"),
						  csp->getf("Scenes.Fourth.Fish.AO.quadraticAtt"));

	rotateVel = csp->getf("Scenes.Fourth.Fish.rotateVel");
	csp->readLights("Scenes.Fourth.Fish.Lights");
	csp->passToLight(lights);
}


FourthStormScene::FourthStormScene(CSParser *csp, Scanner *s, FBO *rL, FBO *rR) : StormScene(s) {
	readConf(csp);

	renderFish = new FourthRendererFish(csp, scan->rig, rL, rR);

	text = TBO("Images/Biotechnopolis/040R.fw.png", true);
	text.clamp(true);

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
}

void FourthStormScene::renderModel() {
	renderFish->draw(0, 0);
}

void FourthStormScene::modelDraw(mat4 *V, mat4 *P, FBO *render, bool left) {
	render->bind(false);
	glDisable(GL_DEPTH_TEST);
	scan->mixShad->use();
	if(left) {
		renderFish->outputBuffL->bind_texture(0, 0);
		renderFish->renderBufferL->bind_depth_texture(1);
	}
	else {
		renderFish->outputBuffR->bind_texture(0, 0);
		renderFish->renderBufferR->bind_depth_texture(1);
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
	render->unbind();
}

void FourthStormScene::linesDraw(mat4 *V, mat4 *P, FBO *render) {
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

void FourthStormScene::textDraw(mat4 *V, mat4 *P, FBO *render) {
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

void FourthStormScene::readConf(CSParser *csp) {
	textHeight	= csp->getf("Scenes.Fourth.textHeight");
	linesHeight	= csp->getf("Scenes.Fourth.linesHeight");
	stillTime	= csp->getf("Scenes.Fourth.stillTime");
}

void FourthStormScene::update() {
	renderFish->setPosition(&scan->cells->cells[scan->scanningCell].p);
}

STATE FourthStormScene::flowControl() {
	STATE now = scan->status;

	if(now == STATE::GRID) return STATE::STILL;
	if(now == STATE::STILL) return STATE::UNSCAN;
	if(now == STATE::UNSCAN) {
		renderFish->rotateAlpha = 0;
		return STATE::REST;
	}
}