#include "Second.h"
#include "../Scanner.h"


SecondRendererInnerPlacoderm::SecondRendererInnerPlacoderm(CSParser *csp, Camera *cam) : Deferred() {
	setup(cam);
	this->csp = csp;

	Inner_3DS = new A3dsHandler("Models/Storm/2PlacodermIN.3DS", 0);
//	Flu_3DS->makeNormalsPerVertex();
	Inner_3DS->readNormalsFromFile("Models/Storm/2PlacodermINNormals.txt");
	Inner_3DS->makeBoundingBox();

	Inner = new Model(firstShad,
					  Inner_3DS->vertexs,
					  Inner_3DS->normals,
					  NULL,
					  NULL,
					  NULL,
					  Inner_3DS->indexs,
					  0.4,
					  vec3(124.0f/255.0f, 114.0f/255.0f, 0.0f),
					  vec3(0.1f, 0.1f, 0.1f),
					  0.01f,
					  &Inner_M[0],
					  InnerSize/Inner_3DS->maxDimension,
					  NULL,
					  NULL);

	dotheAA(true);
	dotheDOF(false);
	dotheAO(2, 0.05, vec2(2, 2), true);

	setValues();

	readConf(csp);
}

void SecondRendererInnerPlacoderm::setValues() {
	zLate = 0.0;
	currentZoomVel = 0.0;
	popCount = 1;
	move = false;
	for(int i=0; i<4; i++) {
		rotationVector[i] = normalize(vec3(randValue(-1.0,1.0), randValue(-1.0, 1.0), randValue(-1.0, 1.0)));
	}
}

void SecondRendererInnerPlacoderm::startMove() {
	move = true;
}

void SecondRendererInnerPlacoderm::setPosition(vec3 *position) {
	pos = position;

	for(int i = 0; i<4; i++) {
		rotate_M[i] = rotate_M[i] * rotate(-1.0f*rotVel, rotationVector[i]);
	}

	if(move) {
		if(currentZoomVel < zoomVel) currentZoomVel += 0.05*zoomVel;
		zLate += currentZoomVel;
	}

	if(popCount < 4 && int(zLate/(2.0*Inner->scale)) > popCount) popCount++;

	vec3 dir = cam->position - *position;
	dir = normalize(dir);

	for(int i=0; i<4; i++) {
		vec4 dd(dir, 0.0);
		dd = glm::rotate(i*360.0f/4.0f, 0.0f, 0.0f, 1.0f)*glm::rotate(-10.0f, 0.0f, 1.0f, 0.0f)*dd;
		Inner_M[i] = glm::translate(i*4.0f, 0.0f, -i*Inner->scale) * glm::translate((zLate-i*Inner->scale)*vec3(dd.x, dd.y, dd.z)) * glm::translate(*position) * rotate_M[i] *
			rotate(-90.0f, 1.0f, 0.0f, 0.0f) * glm::translate(-Inner_3DS->center*Inner->scale);
	}
}

void SecondRendererInnerPlacoderm::render(int s, double t) {
//	Inner->render();
	
	for(int i=0; i<1; i++) {
		Inner->M = &Inner_M[i];
		Inner->render();
	}
}

void SecondRendererInnerPlacoderm::update(double t) {
	readConf(csp);
	setPosition(pos);
}

void SecondRendererInnerPlacoderm::readConf(CSParser *csp) {
	csp->parse();
	InnerSize = csp->getf("Scenes.Second.In.size");
	Inner->scale = InnerSize/Inner_3DS->maxDimension;
	
	Inner->shininess = csp->getf("Scenes.Second.In.shininess");
	Inner->diffuse_color = csp->getvec3("Scenes.Second.In.diffuseColor");
	Inner->specular_color = csp->getvec3("Scenes.Second.In.specularColor");

	AO_radius = csp->getf("Scenes.Second.AO.radius");
	AO_bias   = csp->getf("Scenes.Second.AO.bias");
	AO_attenuation = vec2(csp->getf("Scenes.Second.AO.linearAtt"),
						  csp->getf("Scenes.Second.AO.quadraticAtt"));

	csp->readLights("Scenes.Second.Lights");
	csp->passToLight(lights);

	rotVel = csp->getf("Scenes.Second.In.rotationVel");
	zoomVel = csp->getf("Scenes.Second.In.zoomVel");
}



SecondRendererOutterPlacoderm::SecondRendererOutterPlacoderm(CSParser *csp, Camera *cam, SecondRendererInnerPlacoderm *in) : Deferred() {
	secondShad = new Shader("Shaders/Deferred/second.vert", "Shaders/Vladivostok/Scenes/Placoderm.frag");

	setup(cam);
	this->csp = csp;
	this->in = in;

	Inner_3DS = new A3dsHandler("Models/Storm/2PlacodermOUT.3DS", 0);
	Inner_3DS->readNormalsFromFile("Models/Storm/2PlacodermOUTNormals.txt");
	Inner_3DS->makeUVs();
	Inner_3DS->makeTBNSpace();
	Inner_3DS->makeBoundingBox();

	Inner = new Model(firstShad,
					  Inner_3DS->vertexs,
					  Inner_3DS->normals,
					  Inner_3DS->UVs,
					  Inner_3DS->tangents,
					  Inner_3DS->bitangents,
					  Inner_3DS->indexs,
					  0.4,
					  vec3(124.0f/255.0f, 114.0f/255.0f, 0.0f),
					  vec3(0.1f, 0.1f, 0.1f),
					  0.01f,
					  &Inner_M,
					  InnerSize/Inner_3DS->maxDimension,
					  NULL,
					  "Images/noisec.fw.png");

	dotheAA(true);
	dotheDOF(false);
	dotheAO(2, 0.05, vec2(2, 2), true);

	readConf(csp);
}

void SecondRendererOutterPlacoderm::setPosition(vec3 *position) {
	pos = position;

	rotate_M = rotate_M * rotate(-1.0f*rotVel, 0.0f, 1.0f, 0.0f);

	vec3 dir = cam->position - *position;
	dir = normalize(dir);
	Inner_M = glm::translate(*position) * rotate_M *
			rotate(-90.0f, 1.0f, 0.0f, 0.0f) * glm::translate(-Inner_3DS->center*Inner->scale);
}

void SecondRendererOutterPlacoderm::render(int s, double t) {
	for(int i=0; i<1; i++) {
		Inner->M = &in->Inner_M[i];
		Inner->render();
	}
}

void SecondRendererOutterPlacoderm::update(double t) {
	readConf(csp);
	setPosition(pos);
}

void SecondRendererOutterPlacoderm::readConf(CSParser *csp) {
	csp->parse();
	InnerSize = csp->getf("Scenes.Second.Out.size");
	Inner->scale = InnerSize/Inner_3DS->maxDimension;
	
	Inner->shininess = csp->getf("Scenes.Second.Out.shininess");
	Inner->diffuse_color = csp->getvec3("Scenes.Second.Out.diffuseColor");
	Inner->specular_color = csp->getvec3("Scenes.Second.Out.specularColor");

	AO_radius = csp->getf("Scenes.Second.AO.radius");
	AO_bias   = csp->getf("Scenes.Second.AO.bias");
	AO_attenuation = vec2(csp->getf("Scenes.Second.AO.linearAtt"),
						  csp->getf("Scenes.Second.AO.quadraticAtt"));

	csp->readLights("Scenes.Second.Lights");
	csp->passToLight(lights);

	rotVel = csp->getf("Scenes.Second.In.rotationVel");
}



SecondStormScene::SecondStormScene(CSParser *csp, Scanner *s) : StormScene(s) {
	readConf(csp);

	inner = new SecondRendererInnerPlacoderm(csp, scan->rig);
	outter = new SecondRendererOutterPlacoderm(csp, scan->rig, inner);

	text = TBO("Images/Biotechnopolis/020R.fw.png", true);
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

	mixAlphaShad = new Shader("Shaders/Post/general.vert", "Shaders/Vladivostok/Scenes/PlacodermMix.frag");
	mix_position_Id = mixAlphaShad->getUniform("position");
	mix_TexIn_Id = mixAlphaShad->getUniform("TexIn");
	mix_TexOut_Id = mixAlphaShad->getUniform("TexOut");
	mix_DepthIn_Id = mixAlphaShad->getUniform("DepthIn");
	mix_DepthOut_Id = mixAlphaShad->getUniform("DepthOut");
	mix_OutAlpha_Id = mixAlphaShad->getUniform("alphaValue");
}

void SecondStormScene::renderModel() {
	inner->draw(0, 0);
	outter->draw(0, 0);
}

void SecondStormScene::modelDraw(mat4 *V, mat4 *P, FBO *render, bool left) {
	vec3 direction = normalize(scan->rig->position-scan->cells->cells[scan->scanningCell].p);

	render->bind(false);
	glDisable(GL_DEPTH_TEST);
	mixAlphaShad->use();
	if(left) {
		inner->outputBuffL->bind_texture(0, 0);
		outter->outputBuffL->bind_texture(0, 1);
		inner->renderBufferL->bind_depth_texture(2);
		outter->renderBufferL->bind_depth_texture(3);
	}
	else {
		inner->outputBuffR->bind_texture(0, 0);
		outter->outputBuffR->bind_texture(0, 1);
		inner->renderBufferR->bind_depth_texture(2);
		outter->renderBufferR->bind_depth_texture(3);
	}

	glUniform1i(mix_TexIn_Id, 0);
	glUniform1i(mix_TexOut_Id, 1);
	glUniform1i(mix_DepthIn_Id, 2);
	glUniform1i(mix_DepthOut_Id, 3);
	glUniform1f(mix_OutAlpha_Id, alphaVal);
	
	glUniform1f(mix_position_Id, scan->worldToClip(V, P, &scan->gridPositionVec));
	scan->quad->enable(3);
	scan->quad_I->draw(GL_TRIANGLES);
	scan->quad->disable();
	glEnable(GL_DEPTH_TEST);
	render->unbind();
}

void SecondStormScene::linesDraw(mat4 *V, mat4 *P, FBO *render) {
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

void SecondStormScene::textDraw(mat4 *V, mat4 *P, FBO *render) {
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

void SecondStormScene::readConf(CSParser *csp) {
	textHeight	= csp->getf("Scenes.Second.textHeight");
	linesHeight	= csp->getf("Scenes.Second.linesHeight");
	stillTime	= csp->getf("Scenes.Second.stillTime");
	alphaVal	= csp->getf("Scenes.Second.Out.opacity");
}

void SecondStormScene::update() {
	inner->setPosition(&scan->cells->cells[scan->scanningCell].p);
	outter->setPosition(&scan->cells->cells[scan->scanningCell].p);
}

STATE SecondStormScene::flowControl() {
	STATE now = scan->status;

	if(now == STATE::GRID) {
		inner->startMove();
		return STATE::STILL;
	}
	if(now == STATE::STILL) return STATE::UNSCAN;
	if(now == STATE::UNSCAN) {
		inner->setValues();
		return STATE::REST;
	}
}