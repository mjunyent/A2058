#include "Fifth.h"
#include "../Scanner.h"

FifthStormScene::FifthStormScene(CSParser *csp, Scanner *s) : StormScene(s) {
	cout << "Loading Fifth Scene..." << endl;
	readConf(csp);

	evolution = vector<TBO*>(11);

	std::string filenamePre = "Images/Biotechnopolis/evolution/evolution";
	std::string a = "0";
	std::string b = "0";
	std::string filenamePost = ".png";

	for(int i=0; i<evolution.size(); i++) {
		std::string name = filenamePre + a + b + filenamePost;
		cout << "Loading file " << name << endl;
		evolution[i] = new TBO(name.c_str(), true);
		evolution[i]->clamp(true);
		if(b[0] == '9') {
			b[0] = '0';
			a[0]++;
		} else b[0]++;
	}

	text = TBO("Images/Biotechnopolis/050R.fw.png", true);
	text.clamp(true);

	squareLines = TBO("Images/Biotechnopolis/Lines/SquareRight.fw.png", true);
	squareLines.clamp(true);

	//Make texture Quads:
	float ratio = float(squareLines.width)/float(squareLines.height);
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

	ratio = float(evolution[0]->width)/float(evolution[0]->height);
	float evoWidth = ratio*evoHeight;

	float heartRect[] = {
		 -evoWidth/2.0f,	 evoHeight/2.0f, 0.0f,
		  evoWidth/2.0f,	 evoHeight/2.0f, 0.0f,
		  evoWidth/2.0f,	-evoHeight/2.0f, 0.0f,
		 -evoWidth/2.0f,	-evoHeight/2.0f, 0.0f,
	};

	evoQuad = new VBO(heartRect, sizeof(director::quad), 0);

	evoFrame = 0;
}

void FifthStormScene::renderModel() {
}

void FifthStormScene::modelDraw(mat4 *V, mat4 *P, FBO *render, bool left) {
	vec3 direction = normalize(scan->rig->position-scan->cells->cells[scan->scanningCell].p);

	mat4 M = translate(scan->cells->cells[scan->scanningCell].p
					 + direction*position.z
					 + vec3(position.x, position.y, 0.0f)
					 + displacementVec*dispPos);

	render->bind(false);

	scan->textShad->use();
	jelly[jellyFrame]->bind(0);
	glUniformMatrix4fv(scan->text_M_Id, 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv(scan->text_V_Id, 1, GL_FALSE, &(*V)[0][0]); 
	glUniformMatrix4fv(scan->text_P_Id, 1, GL_FALSE, &(*P)[0][0]);
	glUniform1f(scan->text_sP_Id, (scan->worldToClip(V, P, &scan->gridPositionVec)+1.0f)/2.0f*float(render->width));
	glUniform1i(scan->text_rightTex_Id, 0);
	glUniform1i(scan->text_leftTex_Id, 0);
	glUniform1i(scan->text_showL_Id, 0);
	glUniform1i(scan->text_showR_Id, 1);
	
	jellyQuad->enable(3);
	scan->textQuadCoords->enable(3);
	scan->quad_I->draw(GL_TRIANGLES);
	scan->textQuadCoords->disable();
	jellyQuad->disable();

	render->unbind();
}

void FifthStormScene::linesDraw(mat4 *V, mat4 *P, FBO *render) {
	mat4 M = translate(scan->cells->cells[scan->scanningCell].p);

	render->bind(false);
	scan->textShad->use();
	squareLines.bind(0);
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

void ThirdStormScene::textDraw(mat4 *V, mat4 *P, FBO *render) {
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

void ThirdStormScene::readConf(CSParser *csp) {
	textHeight	= csp->getf("Scenes.Third.textHeight");
	linesHeight	= csp->getf("Scenes.Third.linesHeight");
	stillTime	= csp->getf("Scenes.Third.stillTime");
	jellyHeight = csp->getf("Scenes.Third.jellyHeight");
	position	= csp->getvec3("Scenes.Third.translate");
	displacementVel = csp->getf("Scenes.Third.displacementVel");
	displacementVec = csp->getvec3("Scenes.Third.displacementVec");
}

void ThirdStormScene::update() {
	if(skip) {
		jellyFrame++;
		jellyFrame %= jelly.size();
	}
	skip = !skip;
	dispPos += displacementVel;
}

STATE ThirdStormScene::flowControl() {
	STATE now = scan->status;

	if(now == STATE::GRID) return STATE::STILL;
	if(now == STATE::STILL) return STATE::UNSCAN;
	if(now == STATE::UNSCAN) {
		dispPos = 0.0;
		return STATE::REST;
	}
}