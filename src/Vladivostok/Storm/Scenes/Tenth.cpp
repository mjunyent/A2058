#include "Tenth.h"
#include "../Scanner.h"

TenthStormScene::TenthStormScene(CSParser *csp, Scanner *s) : StormScene(s) {
	cout << "Loading Tenth Scene..." << endl;
	readConf(csp);

	logo = TBO("Images/Biotechnopolis/biotechnopolis_logo_blackandwhite.png", true);
	logo.clamp(true);

	text = vector<TBO*>(3);

	text[0] = new TBO("Images/Biotechnopolis/100R.fw.png", true);
	text[0]->clamp(true);
	text[1] = new TBO("Images/Biotechnopolis/101R.fw.png", true);
	text[1]->clamp(true);
	text[2] = new TBO("Images/Biotechnopolis/102R.fw.png", true);
	text[2]->clamp(true);



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


	ratio = float(text[0]->width)/float(text[0]->height);
	float textWidth = ratio*textHeight;

	float rectText[] = {
		 linesLeftEnd-textWidth,  textHeight/2.0f, 0.0f, //0 UP, LEFT
		 linesLeftEnd,			  textHeight/2.0f, 0.0f, //1 UP, RIGHT
		 linesLeftEnd,			 -textHeight/2.0f, 0.0f, //2 DOWN, RIGHT
		 linesLeftEnd-textWidth, -textHeight/2.0f, 0.0f  //3 DOWN, LEFT
	};

	textQuad = new VBO(rectText, sizeof(director::quad), 0);

	ratio = float(logo.width)/float(logo.height);
	float logoWidth = ratio*logoHeight;

	float heartRect[] = {
		 -logoWidth/2.0f,	 logoHeight/2.0f, 0.0f,
		  logoWidth/2.0f,	 logoHeight/2.0f, 0.0f,
		  logoWidth/2.0f,	-logoHeight/2.0f, 0.0f,
		 -logoWidth/2.0f,	-logoHeight/2.0f, 0.0f,
	};

	logoQuad = new VBO(heartRect, sizeof(director::quad), 0);

	textFrame = 0;
	saveScanSize = 0.0;
}

void TenthStormScene::renderModel() {
}

void TenthStormScene::modelDraw(mat4 *V, mat4 *P, FBO *render, bool left) {
	vec3 direction = normalize(scan->rig->position-scan->cells->cells[scan->scanningCell].p);
	mat4 M = translate(scan->cells->cells[scan->scanningCell].p
					   + direction*logoLate.z
					   + vec3(logoLate.x, logoLate.y, 0.0));

	render->bind(false);
	scan->textShad->use();
	logo.bind(0);
	glUniformMatrix4fv(scan->text_M_Id, 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv(scan->text_V_Id, 1, GL_FALSE, &(*V)[0][0]); 
	glUniformMatrix4fv(scan->text_P_Id, 1, GL_FALSE, &(*P)[0][0]);
	glUniform1f(scan->text_sP_Id, (scan->worldToClip(V, P, &scan->gridPositionVec)+1.0f)/2.0f*float(render->width));
	glUniform1i(scan->text_rightTex_Id, 0);
	glUniform1i(scan->text_leftTex_Id, 0);

	glUniform1i(scan->text_showR_Id, 1);
	if(textFrame < text.size() && textFrame > 0) glUniform1i(scan->text_showL_Id, 1);
	else glUniform1i(scan->text_showL_Id, 0);
	
	logoQuad->enable(3);
	scan->textQuadCoords->enable(3);
	scan->quad_I->draw(GL_TRIANGLES);
	scan->textQuadCoords->disable();
	logoQuad->disable();
	render->unbind();
}

void TenthStormScene::linesDraw(mat4 *V, mat4 *P, FBO *render) {
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
	if(textFrame < text.size() && textFrame > 0) glUniform1i(scan->text_showL_Id, 1);
	else glUniform1i(scan->text_showL_Id, 0);
	glUniform1i(scan->text_showR_Id, 1);
	
	linesQuad->enable(3);
	scan->textQuadCoords->enable(3);
	scan->quad_I->draw(GL_TRIANGLES);
	scan->textQuadCoords->disable();
	linesQuad->disable();

	glEnable(GL_DEPTH_TEST);

	render->unbind();
}

void TenthStormScene::textDraw(mat4 *V, mat4 *P, FBO *render) {
	mat4 M = translate(scan->cells->cells[scan->scanningCell].p);

	render->bind(false);
	glDisable(GL_DEPTH_TEST);
	scan->textShad->use();

	if(textFrame < text.size())		text[textFrame]->bind(1);
	if(textFrame > 0)				text[textFrame-1]->bind(0);

	glUniformMatrix4fv(scan->text_M_Id, 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv(scan->text_V_Id, 1, GL_FALSE, &(*V)[0][0]); 
	glUniformMatrix4fv(scan->text_P_Id, 1, GL_FALSE, &(*P)[0][0]);
	glUniform1f(scan->text_sP_Id, (scan->worldToClip(V, P, &scan->gridPositionVec)+1.0f)/2.0f*float(render->width));

	STATE what = scan->status;
	if(what == STILL) what = last;

	if(what == STATE::UNSCAN) {
		glUniform1i(scan->text_rightTex_Id, 0);
		glUniform1i(scan->text_leftTex_Id, 1);
		if(textFrame < text.size()) glUniform1i(scan->text_showL_Id, 1);
		else glUniform1i(scan->text_showL_Id, 0);
		if(textFrame > 0) glUniform1i(scan->text_showR_Id, 1);
		else glUniform1i(scan->text_showR_Id, 0);	
	} else {
		glUniform1i(scan->text_rightTex_Id, 1);
		glUniform1i(scan->text_leftTex_Id, 0);
		if(textFrame < text.size()) glUniform1i(scan->text_showR_Id, 1);
		else glUniform1i(scan->text_showR_Id, 0);
		if(textFrame > 0) glUniform1i(scan->text_showL_Id, 1);
		else glUniform1i(scan->text_showL_Id, 0);	
	}

	textQuad->enable(3);
	scan->textQuadCoords->enable(3);
	scan->quad_I->draw(GL_TRIANGLES);
	scan->textQuadCoords->disable();
	textQuad->disable();

	render->unbind();
}

void TenthStormScene::readConf(CSParser *csp) {
	textHeight	 = csp->getf("Scenes.Tenth.textHeight");
	linesHeight	 = csp->getf("Scenes.Tenth.linesHeight");
	stillTime	 = csp->getf("Scenes.Tenth.stillTime");
	logoHeight	 = csp->getf("Scenes.Tenth.logoHeight");
	logoLate	 = csp->getvec3("Scenes.Tenth.logoLate");
}

void TenthStormScene::update() {

}

STATE TenthStormScene::flowControl() {
	STATE now = scan->status;

	if(textFrame == 0)
		stillTime = 7.0;
	else if(textFrame == 1)
		stillTime = 8.0;
	else
		stillTime = 9.0;
		

	if(now == STATE::GRID) {
		scan->cells->slowGo = 0.01;
		last = now;
		scan->side = 3;
		saveDelRad = scan->gridDeleteRadius;
		scan->gridDeleteRadius = 0.0f;
		return STATE::STILL;
	}
	if(now == STATE::STILL) {
		textFrame++;
		//if(evoFrame == evolution.size()) scan->scanSize = saveScanSize;
		if(last == STATE::GRID) return STATE::UNSCAN;
		if(last == STATE::UNSCAN) return STATE::GRID;
	}
	if(now == STATE::UNSCAN) {
		if(textFrame == text.size()) {
			textFrame = 0;
			scan->side = 1;
			scan->gridDeleteRadius = saveDelRad;
			scan->cells->slowGo = 0.02;
			//saveScanSize = 0.0;
			return STATE::REST;
		}
/*		if(saveScanSize == 0.0) {
			saveScanSize = scan->scanSize;
			scan->scanSize = evoWidth+scan->distanceFade;
		}*/
		last = now;
		return STATE::STILL;
	}
}