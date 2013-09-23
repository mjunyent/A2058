#include "Scanner.h"

Scanner::Scanner(CSParser *csp, Cells *cells, Rig *rig) {
	this->cells = cells;
	grid = new Grid(csp->getf("Scan.Grid.size"), csp->getf("Scan.Grid.divisions"));
	readConf(csp);

	scanningCell = -1;
	status = DETECTING;

	bool lecalite[] = { true };
	impas = new FBO(csp->data.width, csp->data.height, true, 1, lecalite);

	//Prepare Shader
	gridShad = new Shader("Shaders/Vladivostok/stormScan.vert", "Shaders/Vladivostok/stormScan.frag");
	grid_M_Id = gridShad->getUniform("Model");
	grid_V_Id = gridShad->getUniform("View");
	grid_P_Id = gridShad->getUniform("Projection");
	grid_centerPosition_Id = gridShad->getUniform("centerPosition");
	grid_radius_Id = gridShad->getUniform("radius");
	grid_alpha_Id = gridShad->getUniform("alpha");

	mixShad = new Shader("Shaders/Post/general.vert", "Shaders/Vladivostok/stormMix.frag");
	mix_position_Id = mixShad->getUniform("position");
	mix_Tex_Id = mixShad->getUniform("Tex");
	mix_Depth_Id = mixShad->getUniform("Depth");

	textShad = new Shader("Shaders/Vladivostok/stormText.vert", "Shaders/Vladivostok/stormText.frag");
	text_M_Id = textShad->getUniform("Model");
	text_V_Id = textShad->getUniform("View");
	text_P_Id = textShad->getUniform("Projection");
	text_sP_Id = textShad->getUniform("screenPosition");
	text_image_Id = textShad->getUniform("image");
	text_show_Id = textShad->getUniform("show");

	quad = new VBO(director::quad, sizeof(director::quad), 0);
	quad_I = new IBO(director::quad_I, sizeof(director::quad_I));
	textQuadCoords = new VBO(director::quad, sizeof(director::quad), 1);

	this->rig = rig;

	linesCircleLeft  = TBO("Images/Biotechnopolis/Lines/CircleLeft.fw.png", true);
	linesCircleLeft.clamp(true);
	linesCircleRight = TBO("Images/Biotechnopolis/Lines/CircleRight.fw.png", true);
	linesCircleRight.clamp(true);

	first = new FirstStormScene(csp, this);
}

void Scanner::detect() {
	scanningCell = -1;

	for(int i=0; i<cells->cells.size(); i++) {
		if(inRange(cells->cells[i].p.x, upLeftNear.x, downRightFar.x) &&
		   inRange(cells->cells[i].p.y, upLeftNear.y, downRightFar.y) &&
		   inRange(cells->cells[i].p.z, upLeftNear.z, downRightFar.z)) {
			   if(scanningCell != -1) { //if it's the second we detect.
				   scanningCell = -1;
				   return;
			   }
			   else scanningCell = i;
		}
	}
}

void Scanner::renderModel() {
	if(status == GRID || status == STILL || status == UNSCAN) {
		first->renderModel();
	}
}

void Scanner::drawModel(mat4 *V, mat4 *P, FBO *render, bool left) {
	first->modelDraw(V, P, render, left);
}

void Scanner::drawText(mat4 *V, mat4 *P, FBO *render) {
	first->textDraw(V, P, render);
}

void Scanner::drawGrid(mat4 *V, mat4 *P, FBO *render) {
	if(status != GRID && status != UNSCAN) return;
	mat4 idd = translate(gridPositionVec);
	float alpha = 1.0;
	if(inRange(gridPosition, 0.0f, distanceFade)) {
		alpha = gridPosition/distanceFade;
	} else if(inRange(gridPosition, scanSize, scanSize-distanceFade)) {
		alpha = (scanSize-gridPosition)/distanceFade;
	}

	render->bind(false);
	gridShad->use();
	glUniformMatrix4fv(grid_M_Id, 1, GL_FALSE, &idd[0][0]);
	glUniformMatrix4fv(grid_V_Id, 1, GL_FALSE, &(*V)[0][0]); 
	glUniformMatrix4fv(grid_P_Id, 1, GL_FALSE, &(*P)[0][0]);
	glUniform3fv(grid_centerPosition_Id, 1, &cells->cells[scanningCell].p[0]);
	glUniform1f(grid_radius_Id, gridDeleteRadius);
	glUniform1f(grid_alpha_Id, alpha);
	grid->render();
	render->unbind();

}

float Scanner::draw(mat4 *V, mat4 *P, FBO *render, bool left) {
	if(status == GRID || status == STILL || status == UNSCAN) {
		first->linesDraw(V, P, render);
		drawModel(V, P, render, left);
		drawText(V, P, render);
		drawGrid(V, P, render);

		return worldToClip(V, P, &gridPositionVec);
	}
	return 2;
}

float Scanner::worldToClip(mat4 *V, mat4 *P, vec3 *v) {
	vec4 screenPosition = *P * *V * translate(*v) * vec4(0.0, 0.0, 0.0, 1.0);
	return screenPosition.x/screenPosition.w;
}

void Scanner::update() {
	if(status == REST) {
		if(director::currentTime - lastTime > restTime) {
			status = DETECTING;
			lastTime = director::currentTime;
		}
	} else if(status == DETECTING) {
		detect();
		if(scanningCell != -1) {
			status = START;
			cells->select(scanningCell);
//			cells->slowStop(scanningCell);
			lastTime = director::currentTime;
		}
	} else if(status == START) {
		if(!cells->move) {
//		if(director::currentTime-lastTime > startTime) {
			status = GRID;
			gridPositionVec = cells->cells[scanningCell].p;
			gridPositionVec.x += scanStart;
			gridPosition = 0.0; 
			lastTime = director::currentTime;
		}
	} else if(status == GRID) {
		gridPositionVec.x -= gridVelocity;
		gridPosition += gridVelocity;
		if(gridPosition > scanSize) {
			status = STILL;
			lastTime = director::currentTime;
		}
	} else if(status == STILL) {
		if(director::currentTime-lastTime > stillTime) {
			status = UNSCAN;
			lastTime = director::currentTime;
		}
	} else if(status == UNSCAN) {
		gridPositionVec.x += gridVelocity;
		gridPosition -= gridVelocity;
		if(gridPosition <= 0.0) {
			status = REST;
			cells->Play();
			lastTime = director::currentTime;
		}
	}
}

void Scanner::debSetup() {
	debShad = new Shader("Shaders/debugLines.vert", "Shaders/debugLines.frag");
	deb_M_Id		 = debShad->getUniform("Model");
	deb_V_Id		 = debShad->getUniform("View");
	deb_P_Id		 = debShad->getUniform("Projection");
	deb_Color_Id	 = debShad->getUniform("Color");
	debBox = NULL;
}

void Scanner::renderDebugBox(glm::mat4 *M, glm::mat4 *V, glm::mat4 *P) {
	float left = upLeftNear.y;
	float right = downRightFar.y;
	float up = upLeftNear.x;
	float down = downRightFar.x;
	float near = upLeftNear.z;
	float far = downRightFar.z;

	float quadFront[] = { 
		up, left, near,	//0 UP, LEFT
		up, right, near, //1 UP, RIGHT

		up, right, near, //1 UP, RIGHT
		down, right, near, //2 DOWN, RIGHT

		down, right, near, //2 DOWN, RIGHT
		down, left, near,  //3 DOWN, LEFT

		down, left, near,  //3 DOWN, LEFT
		up, left, near,	//0 UP, LEFT

		up, left, far,	//0 UP, LEFT
		up, right, far, //1 UP, RIGHT

		up, right, far, //1 UP, RIGHT
		down, right, far, //2 DOWN, RIGHT

		down, right, far, //2 DOWN, RIGHT
		down, left, far,  //3 DOWN, LEFT

		down, left, far,  //3 DOWN, LEFT
		up, left, far	//0 UP, LEFT
	};

	if(debBox != NULL) debBox->destroy();

	debBox = new VBO(quadFront, sizeof(quadFront), 0);

	debShad->use();
	glUniformMatrix4fv(deb_M_Id, 1, GL_FALSE, &(*M)[0][0]);
	glUniformMatrix4fv(deb_V_Id, 1, GL_FALSE, &(*V)[0][0]);
	glUniformMatrix4fv(deb_P_Id, 1, GL_FALSE, &(*P)[0][0]);
	glUniform3f(deb_Color_Id, 1.0, 1.0, 1.0);

	debBox->enable(3);
	debBox->draw(GL_LINES);
	debBox->disable();
}

void Scanner::readConf(CSParser *csp) {
	upLeftNear = vec3(csp->getf("Scan.box.left"), csp->getf("Scan.box.up"), csp->getf("Scan.box.near"));
	downRightFar = vec3(csp->getf("Scan.box.right"), csp->getf("Scan.box.down"), csp->getf("Scan.box.far"));

	restTime = csp->getf("Scan.restTime");
	startTime = csp->getf("Scan.startTime");
	stillTime = csp->getf("Scan.stillTime");
	gridVelocity = csp->getf("Scan.gridVelocity");
	scanSize = csp->getf("Scan.scanSize");
	scanStart = csp->getf("Scan.scanStart");
	gridDeleteRadius = csp->getf("Scan.deleteRadius");
	scanTextStart = csp->getf("Scan.scanTextStart");
	distanceFade = csp->getf("Scan.distanceFade");
}






Grid::Grid(float size, int divisions) {
	//Do the quad.
	pushVertex(0.0, size/2.0, -size/2.0);
	pushVertex(0.0, size/2.0,  size/2.0);

	pushVertex(0.0, -size/2.0, -size/2.0);
	pushVertex(0.0, -size/2.0,  size/2.0);

	pushVertex(0.0, -size/2.0, size/2.0);
	pushVertex(0.0,  size/2.0, size/2.0);

	pushVertex(0.0, -size/2.0, -size/2.0);
	pushVertex(0.0,  size/2.0, -size/2.0);

	float step = size/(divisions+1);

	for(int i=1; i<=divisions; i++) {
		pushVertex(0.0,  -size/2.0+i*step, -size/2.0);
		pushVertex(0.0,  -size/2.0+i*step,  size/2.0);

		pushVertex(0.0,  -size/2.0, -size/2.0+i*step);
		pushVertex(0.0,   size/2.0, -size/2.0+i*step);
	}

	lines = new VBO(lineline, 0);
}

void Grid::render() {
	lines->enable(3);
	lines->draw(GL_LINES);
	lines->disable();
}

void Grid::pushVertex(float x, float y, float z) {
	lineline.push_back(x);
	lineline.push_back(y);
	lineline.push_back(z);
}