#include "Ninth.h"
#include "../Scanner.h"

NinthRendererFlu::NinthRendererFlu(CSParser *csp, Camera *cam, FBO *rL, FBO *rR) : Deferred() {
	secondShad = new Shader("Shaders/Deferred/second.vert", "Shaders/Vladivostok/Scenes/Flu.frag");
	renderBufferL = rL;
	renderBufferR = rR;

	setup(cam);
	this->csp = csp;

	Flu_3DS = new A3dsHandler("Models/Storm/9Flu.3DS", 0);
//	Flu_3DS->makeNormalsPerVertex();
	Flu_3DS->readNormalsFromFile("Models/Storm/9FluNormals.txt");
	Flu_3DS->makeUVs();
	Flu_3DS->makeTBNSpace();
	Flu_3DS->makeBoundingBox();

	Flu = new Model(firstShad,
					Flu_3DS->vertexs,
					Flu_3DS->normals,
					Flu_3DS->UVs,
					Flu_3DS->tangents,
					Flu_3DS->bitangents,
					Flu_3DS->indexs,
					0.2,
					vec3(124.0f/255.0f, 114.0f/255.0f, 0.0f),
					vec3(0.1f, 0.1f, 0.1f),
					0.1f,
					&Flu_M,
					FluSize/Flu_3DS->maxDimension,
					NULL,
					"Images/estosiquesonnormalesguarras.fw.png");

	dotheAA(true);
	dotheDOF(false);
	dotheAO(2, 0.05, vec2(2, 2), true);

	readConf(csp);
	zPos = zLate;
}

void NinthRendererFlu::setPosition(vec3 *position) {
	pos = position;

	rotate_M = rotate_M * rotate(-1.0f*rotateVel, 0.7f, 1.0f, 0.0f);

	vec3 dir = cam->position - *position;
	dir = normalize(dir);
	zPos += zVel;
	Flu_M = glm::translate(zPos*dir) * glm::translate(*position) * rotate_M *
			rotate(-90.0f, 1.0f, 0.0f, 0.0f) * glm::translate(-Flu_3DS->center*Flu->scale);
}

void NinthRendererFlu::render(int s, double t) {
	Flu->render();
}

void NinthRendererFlu::update(double t) {
	readConf(csp);
	setPosition(pos);
}

void NinthRendererFlu::readConf(CSParser *csp) {
	csp->parse();
	FluSize = csp->getf("Scenes.Ninth.Flu.size");
	Flu->scale = FluSize/Flu_3DS->maxDimension;
	
	Flu->shininess = csp->getf("Scenes.Ninth.Flu.shininess");
	Flu->diffuse_color = csp->getvec3("Scenes.Ninth.Flu.diffuseColor");
	Flu->specular_color = csp->getvec3("Scenes.Ninth.Flu.specularColor");

	zLate = csp->getf("Scenes.Ninth.Flu.zLate");

	AO_radius = csp->getf("Scenes.Ninth.Flu.AO.radius");
	AO_bias   = csp->getf("Scenes.Ninth.Flu.AO.bias");
	AO_attenuation = vec2(csp->getf("Scenes.Ninth.Flu.AO.linearAtt"),
						  csp->getf("Scenes.Ninth.Flu.AO.quadraticAtt"));

	csp->readLights("Scenes.Ninth.Flu.Lights");
	csp->passToLight(lights);

	rotateVel = csp->getf("Scenes.Ninth.Flu.rotateVel");
	zVel = csp->getf("Scenes.Ninth.Flu.velocity");
}




NinthStormScene::NinthStormScene(CSParser *csp, Scanner *s, FBO *rL, FBO *rR) : StormScene(s) {
	readConf(csp);

	renderFlu = new NinthRendererFlu(csp, scan->rig, rL, rR);

	text = TBO("Images/Biotechnopolis/090R.fw.png", true);
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

void NinthStormScene::renderModel() {
	renderFlu->draw(0, 0);
}

void NinthStormScene::modelDraw(mat4 *V, mat4 *P, FBO *render, bool left) {
	render->bind(false);
	glDisable(GL_DEPTH_TEST);
	scan->mixShad->use();
	if(left) {
		renderFlu->outputBuffL->bind_texture(0, 0);
		renderFlu->renderBufferL->bind_depth_texture(1);
	}
	else {
		renderFlu->outputBuffR->bind_texture(0, 0);
		renderFlu->renderBufferR->bind_depth_texture(1);
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

void NinthStormScene::linesDraw(mat4 *V, mat4 *P, FBO *render) {
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

void NinthStormScene::textDraw(mat4 *V, mat4 *P, FBO *render) {
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

void NinthStormScene::update() {
	renderFlu->setPosition(&scan->cells->cells[scan->scanningCell].p);
}

void NinthStormScene::readConf(CSParser *csp) {
	textHeight	= csp->getf("Scenes.Ninth.textHeight");
	linesHeight	= csp->getf("Scenes.Ninth.linesHeight");
	stillTime	= csp->getf("Scenes.Ninth.stillTime");
}

STATE NinthStormScene::flowControl() {
	STATE now = scan->status;

	if(now == STATE::GRID) return STATE::STILL;
	if(now == STATE::STILL) return STATE::UNSCAN;
	if(now == STATE::UNSCAN) {
		renderFlu->zPos = renderFlu->zLate;
		return STATE::REST;
	}
}