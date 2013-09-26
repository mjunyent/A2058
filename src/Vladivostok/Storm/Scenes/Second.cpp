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
					  &Inner_M,
					  InnerSize/Inner_3DS->maxDimension,
					  NULL,
					  NULL);

	dotheAA(true);
	dotheDOF(false);
	dotheAO(2, 0.05, vec2(2, 2), true);

	readConf(csp);
}

void SecondRendererInnerPlacoderm::setPosition(vec3 *position) {
	pos = position;

	rotate_M = rotate_M * rotate(-1.0f*rotVel, 0.0f, 1.0f, 0.0f);

	vec3 dir = cam->position - *position;
	dir = normalize(dir);
	Inner_M = glm::translate(*position) * rotate_M *
			rotate(-90.0f, 1.0f, 0.0f, 0.0f) * glm::translate(-Inner_3DS->center*Inner->scale);
}

void SecondRendererInnerPlacoderm::render(int s, double t) {
	Inner->render();
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
}

/*
EightStormScene::EightStormScene(CSParser *csp, Scanner *s) : StormScene(s) {
	readConf(csp);

	renderRBC = new EightRendererRBC(csp, scan->rig);

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
}

void EightStormScene::renderModel() {
	renderRBC->draw(0, 0);
}

void EightStormScene::modelDraw(mat4 *V, mat4 *P, FBO *render, bool left) {
	vec3 direction = normalize(scan->rig->position-scan->cells->cells[scan->scanningCell].p);

	mat4 M = translate(scan->cells->cells[scan->scanningCell].p + direction*(zLate + heartPosition));

	render->bind(false);
	//if(heartAlpha > 0.5) {
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
		renderRBC->setRotVals();
		heartZoom = true;
		return STATE::STILL;
	}
	if(now == STATE::STILL) return STATE::UNSCAN;
	if(now == STATE::UNSCAN) {
		heartZoom = false;
		heartPosition = 0.0;
		heartVel = 0.0;
		heartAlpha = 0.0;
		return STATE::REST;
	}
}*/