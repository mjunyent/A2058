#include "First.h"
#include "../Scanner.h"

FirstRenderer::FirstRenderer(Camera *cam) : Deferred() {
	setup(cam);

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
					  25.0/World_3DS->maxDimension,
					  NULL,
					  NULL);

	//lights->addDirectionalLight(glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, 1.0, -1.0), glm::vec3(1.0, 1.0, 1.0));
}

void FirstRenderer::setPosition(vec3 *position) {
	rotate_M = rotate_M * rotate(-2.0f, 0.0f, 1.0f, 0.0f);
	World_M = glm::translate(*position) * glm::translate(-World_3DS->center*World->scale) * rotate_M * rotate(-90.0f, 1.0f, 0.0f, 0.0f);
}

void FirstRenderer::render(int s, double t) {
	World->render();
}





FirstStormScene::FirstStormScene(CSParser *csp, Scanner *s) : StormScene(s) {
	readConf(csp);

	renderF = new FirstRenderer(scan->rig);

	float rectText[] = { 
		-textWidth/2.0f,  textHeight/2.0f, 0.0f, //0 UP, LEFT
		 textWidth/2.0f,  textHeight/2.0f, 0.0f, //1 UP, RIGHT
		 textWidth/2.0f, -textHeight/2.0f, 0.0f, //2 DOWN, RIGHT
		-textWidth/2.0f, -textHeight/2.0f, 0.0f  //3 DOWN, LEFT
	};

	textQuad = new VBO(rectText, sizeof(director::quad), 0);

//	worldText = TBO("Images/Biotechnopolis/First/
}

void FirstStormScene::renderModel() {
	renderF->setPosition(&scan->cells->cells[scan->scanningCell].p);
	renderF->draw(0, 0);
}

void FirstStormScene::modelDraw(mat4 *V, mat4 *P, FBO *render, bool left) {
	render->bind(false);
	glDisable(GL_DEPTH_TEST);
	scan->mixShad->use();
	if(left) {
		renderF->outputBuffL->bind_texture(0, 0);
		renderF->renderBufferL->bind_depth_texture(1);
	}
	else {
		renderF->outputBuffR->bind_texture(0, 0);
		renderF->renderBufferL->bind_depth_texture(1);
	}
	glUniform1i(scan->mix_Tex_Id, 0);
	glUniform1i(scan->mix_Depth_Id, 1);
	glUniform1f(scan->mix_position_Id, scan->worldToClip(V, P, &scan->gridPositionVec));
	scan->quad->enable(3);
	scan->quad_I->draw(GL_TRIANGLES);
	scan->quad->disable();
	glEnable(GL_DEPTH_TEST);
	render->unbind();
}

//MOVE TEXQUAD here!
void FirstStormScene::textDraw(mat4 *V, mat4 *P, FBO *render) {
	mat4 textM = translate(scan->cells->cells[scan->scanningCell].p-vec3(textOffset, 0.0, 0.0));
	/*
	render->bind(false);
	scan->textShad->use();
	scan->text.bind(0);
	glUniformMatrix4fv(scan->text_M_Id, 1, GL_FALSE, &textM[0][0]);
	glUniformMatrix4fv(scan->text_V_Id, 1, GL_FALSE, &(*V)[0][0]); 
	glUniformMatrix4fv(scan->text_P_Id, 1, GL_FALSE, &(*P)[0][0]);
	glUniform1f(scan->text_sP_Id, (scan->worldToClip(V, P, &scan->gridPositionVec)+1.0f)/2.0f*float(render->width));
	glUniform1i(scan->text_image_Id, 0);
	textQuad->enable(3);
	scan->textQuadCoords->enable(3);
	scan->quad_I->draw(GL_TRIANGLES);
	scan->textQuadCoords->disable();
	textQuad->disable();
	render->unbind();*/
}

void FirstStormScene::readConf(CSParser *csp) {
	textWidth  = csp->getf("Scenes.First.textWidth");
	textHeight = csp->getf("Scenes.First.textHeight");
	textOffset = csp->getf("Scenes.First.textOffset");
}