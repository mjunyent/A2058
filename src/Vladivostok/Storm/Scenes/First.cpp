#include "First.h"
#include "../Scanner.h"

FirstRendererWorld::FirstRendererWorld(CSParser *csp, Camera *cam) : Deferred() {
	setup(cam);

	readConf(csp);

	World_3DS = new A3dsHandler("Models/Storm/1World.3DS");
	World_3DS->makeVBO(0, 1);
//	World_3DS->makeNormalsPerFace();
//	World_3DS->makeNormalsPerVertex();
	World_3DS->makeBoundingBox();
//	World_3DS->makeIndexsByMaterial();

	World_M = glm::translate(-World_3DS->center);
	rotate_M = glm::translate(0.0, 0.0, 0.0);

	World = new Model(firstShad,
					  World_3DS->vertexs,
					  NULL,
					  NULL,
					  NULL,
					  NULL,
					  World_3DS->indexs,
					  0.8,
					  glm::vec3(1.0f, 1.0f, 1.0f),
					  glm::vec3(0.0f, 0.0f, 0.0f),
					  0.2f,
					  &World_M,
					  WorldSize/World_3DS->maxDimension,
					  NULL,
					  NULL);

	lights->addDirectionalLight(glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, 1.0, -1.0), glm::vec3(1.0, 1.0, 1.0));
}

void FirstRendererWorld::setPosition(vec3 *position) {
	rotate_M = rotate_M * rotate(-2.0f, 0.0f, 1.0f, 0.0f);
	vec3 dir = cam->position - *position;
	dir = normalize(dir);

	World_M = glm::translate(zLate*dir) * glm::translate(*position) * rotate_M * rotate(-90.0f, 1.0f, 0.0f, 0.0f) * glm::translate(-World_3DS->center*World->scale);
}

void FirstRendererWorld::render(int s, double t) {
	World->render();
}

void FirstRendererWorld::readConf(CSParser *csp) {
	WorldSize = csp->getf("Scenes.First.World.size");
	zLate = csp->getf("Scenes.First.zLate");
}


FirstRenderPolio::FirstRenderPolio(CSParser *csp, Camera *cam) : Deferred() {
	secondShad = new Shader("Shaders/Deferred/second.vert", "Shaders/Vladivostok/Scenes/Polio.frag");

	setup(cam);
	Polio = NULL;

	Polio_3DS = new A3dsHandler("Models/Storm/1Polio.3DS", 0);
	Polio_3DS->readNormalsFromFile("Models/Storm/1PolioNormals.txt");
//	Polio_3DS->makeNormalsPerVertex();
	Polio_3DS->makeBoundingBox();

	Polio = new Model(firstShad,
					  Polio_3DS->vertexs,
					  Polio_3DS->normals,
					  NULL,
					  NULL,
					  NULL,
					  Polio_3DS->indexs,
					  0.4,
					  vec3(124.0f/255.0f, 114.0f/255.0f, 0.0f),
					  vec3(0.1f, 0.1f, 0.1f),
					  0.01f,
					  &Polio_M,
					  PolioSize/Polio_3DS->maxDimension,
					  NULL,
					  NULL);

	dotheAA(true);
	dotheDOF(false);
	dotheAO(2, 0.05, vec2(2, 2), true);

	readConf(csp);

	lights->addDirectionalLight(glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
}

void FirstRenderPolio::setPosition(vec3 *position) {
	rotate_M = rotate_M * rotate(-2.0f, 0.0f, 1.0f, 0.0f);
	//glm::translate(-World_3DS->center*World->scale) *
	vec3 dir = cam->position - *position;
	dir = normalize(dir);
	Polio_M = glm::translate(zLate*dir) * glm::translate(*position) * rotate_M * rotate(-90.0f, 1.0f, 0.0f, 0.0f) * glm::translate(-Polio_3DS->center*Polio->scale);
}

void FirstRenderPolio::render(int s, double t) {
	Polio->render();
}

void FirstRenderPolio::readConf(CSParser *csp) {
	zLate = csp->getf("Scenes.First.zLate");
	PolioSize = csp->getf("Scenes.First.Polio.size");
	if(Polio != NULL) Polio->scale = PolioSize/Polio_3DS->maxDimension;
	AO_radius = csp->getf("Scenes.First.Polio.AO.radius");
	AO_bias   = csp->getf("Scenes.First.Polio.AO.bias");
	AO_attenuation = vec2(csp->getf("Scenes.First.Polio.AO.linearAtt"),
						  csp->getf("Scenes.First.Polio.AO.quadraticAtt"));
}


FirstStormScene::FirstStormScene(CSParser *csp, Scanner *s) : StormScene(s) {
	readConf(csp);

	renderFw = new FirstRendererWorld(csp, scan->rig);
	renderFp = new FirstRenderPolio(csp, scan->rig);

	firstStill = true;

	worldText = TBO("Images/Biotechnopolis/010R.fw.png", true);
	polioText = TBO("Images/Biotechnopolis/011R.fw.png", true);
	worldText.clamp(true);
	polioText.clamp(true);

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

	ratio = float(worldText.width)/float(worldText.height);
	float textWidth = ratio*textHeight;

	float rectText[] = {
		 linesLeftEnd-textWidth,  textHeight/2.0f, 0.0f, //0 UP, LEFT
		 linesLeftEnd,			  textHeight/2.0f, 0.0f, //1 UP, RIGHT
		 linesLeftEnd,			 -textHeight/2.0f, 0.0f, //2 DOWN, RIGHT
		 linesLeftEnd-textWidth, -textHeight/2.0f, 0.0f  //3 DOWN, LEFT
	};

	textQuad = new VBO(rectText, sizeof(director::quad), 0);
}

void FirstStormScene::renderModel() {
	renderFw->setPosition(&scan->cells->cells[scan->scanningCell].p);
	renderFp->setPosition(&scan->cells->cells[scan->scanningCell].p);

	renderFw->draw(0, 0);
	if(scan->status == STATE::UNSCAN || !firstStill) renderFp->draw(0, 0);
}

void FirstStormScene::modelDraw(mat4 *V, mat4 *P, FBO *render, bool left) {
	render->bind(false);
	glDisable(GL_DEPTH_TEST);
	scan->mixShad->use();
	if(left) {
		renderFw->outputBuffL->bind_texture(0, 0);
		renderFw->renderBufferL->bind_depth_texture(1);
	}
	else {
		renderFw->outputBuffR->bind_texture(0, 0);
		renderFw->renderBufferL->bind_depth_texture(1);
	}
	if(scan->status == STATE::UNSCAN || !firstStill) {
		if(left) {
			renderFp->outputBuffL->bind_texture(0, 2);
			renderFp->renderBufferL->bind_depth_texture(3);
		}
		else {
			renderFp->outputBuffR->bind_texture(0, 2);
			renderFp->renderBufferL->bind_depth_texture(3);
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
	render->unbind();
}

void FirstStormScene::linesDraw(mat4 *V, mat4 *P, FBO *render) {
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

//MOVE TEXQUAD here!
void FirstStormScene::textDraw(mat4 *V, mat4 *P, FBO *render) {
	mat4 M = translate(scan->cells->cells[scan->scanningCell].p);

	render->bind(false);
	scan->textShad->use();
	worldText.bind(0);
	if(scan->status == STATE::UNSCAN || !firstStill) polioText.bind(1);
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

void FirstStormScene::readConf(CSParser *csp) {
	textHeight	= csp->getf("Scenes.First.textHeight");
	linesHeight	= csp->getf("Scenes.First.linesHeight");
	stillTime	= csp->getf("Scenes.First.stillTime");
}

STATE FirstStormScene::flowControl() {
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