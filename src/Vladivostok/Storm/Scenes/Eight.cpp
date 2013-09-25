#include "Eight.h"
#include "../Scanner.h"

EightStormScene::EightStormScene(CSParser *csp, Scanner *s) : StormScene(s) {
	readConf(csp);

//	renderFlu = new NinthRendererFlu(csp, scan->rig);

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
}

void EightStormScene::renderModel() {
//	renderFlu->setPosition(&scan->cells->cells[scan->scanningCell].p);
//	renderFlu->draw(0, 0);
}

void EightStormScene::modelDraw(mat4 *V, mat4 *P, FBO *render, bool left) {
	vec3 direction = normalize(scan->rig->position-scan->cells->cells[scan->scanningCell].p);

	mat4 M = translate(scan->cells->cells[scan->scanningCell].p + direction*(zLate + heartPosition));

	render->bind(false);
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

/*	render->bind(false);
	glDisable(GL_DEPTH_TEST);
	scan->mixShad->use();
	if(left) {
		renderFlu->outputBuffL->bind_texture(0, 0);
		renderFlu->renderBufferL->bind_depth_texture(1);
	}
	else {
		renderFlu->outputBuffR->bind_texture(0, 0);
		renderFlu->renderBufferL->bind_depth_texture(1);
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
	render->unbind();*/
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
	heartVel += heartAccel; //you could integrate man...
	heartPosition += heartVel;
	heartAlpha = clamp(heartAlpha+heartAlphaVel, 0.0f, 1.0f);
}

STATE EightStormScene::flowControl() {
	STATE now = scan->status;

	if(now == STATE::GRID) return STATE::STILL;
	if(now == STATE::STILL) return STATE::UNSCAN;
	if(now == STATE::UNSCAN) return STATE::REST;
}