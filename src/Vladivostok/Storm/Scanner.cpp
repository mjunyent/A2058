#include "Scanner.h"

Scanner::Scanner(CSParser *csp, Cells *cells, Rig *rig) {
	this->cells = cells;
	grid = new Grid(csp->getf("Scan.Grid.size"), csp->getf("Scan.Grid.divisions"));
	readConf(csp);

	scanningCell = -1;
	status = DETECTING;
	statusChanged = false;

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
	mix_TexL_Id		= mixShad->getUniform("TexLeft");
	mix_DepthL_Id	= mixShad->getUniform("DepthLeft");
	mix_showL_Id	= mixShad->getUniform("showLeft");

	mix_TexR_Id		= mixShad->getUniform("TexRight");
	mix_DepthR_Id	= mixShad->getUniform("DepthRight");
	mix_showR_Id	= mixShad->getUniform("showRight");

	textShad = new Shader("Shaders/Vladivostok/stormText.vert", "Shaders/Vladivostok/stormText.frag");
	text_M_Id = textShad->getUniform("Model");
	text_V_Id = textShad->getUniform("View");
	text_P_Id = textShad->getUniform("Projection");
	text_sP_Id = textShad->getUniform("screenPosition");
	text_leftTex_Id = textShad->getUniform("leftTex");
	text_rightTex_Id = textShad->getUniform("rightTex");
	text_showL_Id = textShad->getUniform("showLeft");
	text_showR_Id = textShad->getUniform("showRight");

	quad = new VBO(director::quad, sizeof(director::quad), 0);
	quad_I = new IBO(director::quad_I, sizeof(director::quad_I));
	textQuadCoords = new VBO(director::quad, sizeof(director::quad), 1);

	this->rig = rig;

	linesCircleLeft  = TBO("Images/Biotechnopolis/Lines/CircleLeft.fw.png", true);
	linesCircleLeft.clamp(true);
	linesCircleRight = TBO("Images/Biotechnopolis/Lines/CircleRight.fw.png", true);
	linesCircleRight.clamp(true);

	currentScene = 0;

	bool calite[] = { true, true, true };
	renderBufferL = new FBO(rig->width, rig->height, true, 3, calite);
	renderBufferR = new FBO(rig->width, rig->height, true, 3, calite);

//	scenes.push_back( new FirstStormScene(csp, this, renderBufferL, renderBufferR) );
	scenes.push_back( new SecondStormScene(csp, this) );
//	scenes.push_back( new ThirdStormScene(csp, this) );
//	scenes.push_back( new FourthStormScene(csp, this, renderBufferL, renderBufferR) );
//	scenes.push_back( new FifthStormScene(csp, this) );
//	scenes.push_back( new SixSeventhStormScene(csp, this, renderBufferL, renderBufferR) );
//	scenes.push_back( new EightStormScene(csp, this, renderBufferL, renderBufferR) );
//	scenes.push_back( new NinthStormScene(csp, this, renderBufferL, renderBufferR) );
//	scenes.push_back( new TenthStormScene(csp, this) );
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
		scenes[currentScene]->renderModel();
	}
}

void Scanner::drawModel(mat4 *V, mat4 *P, FBO *render, bool left) {
	scenes[currentScene]->modelDraw(V, P, render, left);
}

void Scanner::drawText(mat4 *V, mat4 *P, FBO *render) {
	scenes[currentScene]->textDraw(V, P, render);
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
		scenes[currentScene]->linesDraw(V, P, render);
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
	if(status == REST) { //NO CELL DETECTION
		gridPositionRLT = 0;
		if(statusChanged) {
			currentScene++;
			currentScene %= scenes.size();

			cells->Play();
			statusChanged = false;
			lastTime = director::currentTime;
		}

		if(director::currentTime - lastTime > restTime) {
			status = DETECTING;
			statusChanged = true;
		}


	} else if(status == DETECTING) { //LOOKING FOR A CELL
		if(statusChanged) {
			statusChanged = false;
			lastTime = director::currentTime;
		}

		detect();

		if(scanningCell != -1) {
			status = START;
			statusChanged = true;
		}


	} else if(status == START) { //STOPPING CELLS and APPROACHING TO SELECTED CELL
		if(statusChanged) {
			cells->select(scanningCell);
//			cells->slowStop(scanningCell);
			statusChanged = false;
			lastTime = director::currentTime;
		}

		if(!cells->move && director::currentTime-lastTime > startTime) {
			status = GRID;
			statusChanged = true;

			gridPositionVec = cells->cells[scanningCell].p;
			gridPositionVec.x += scanStart;
			gridPosition = 0.0;
			side = 1;
		}


	} else if(status == GRID) { //MOVING THE GRID FROM RIGHT TO LEFT
		gridPositionRLT = 2;
		if(statusChanged) {
			gridPositionVec = cells->cells[scanningCell].p;
			gridPositionVec.x += scanStart;
			gridPosition = 0.0;
			
			statusChanged = false;
			lastTime = director::currentTime;
		}

		gridPositionVec.x -= gridVelocity;
		gridPosition += gridVelocity;

		if(gridPosition > scanSize) {
			gridPositionRLT = 1;
			status = scenes[currentScene]->flowControl();
			statusChanged = true;
		}


	} else if(status == STILL) { //STILL TIME DISPLAYING THE SCAN RENDER
		if(statusChanged) {
			if(gridPositionRLT == 1) gridPositionVec.x -= 1000;
			if(gridPositionRLT == 0) gridPositionVec.x += 1000;
//			gridPosition *= 1000;
//			gridPositionVec.x *= 1000;
			statusChanged = false;
			lastTime = director::currentTime;
		}

		if(director::currentTime-lastTime > scenes[currentScene]->stillTime) {
			status = scenes[currentScene]->flowControl();
			statusChanged = true;
		}


	} else if(status == UNSCAN) { //MOVING THE GRID FROM LEFT TO RIGHT
		gridPositionRLT = 2;
		if(statusChanged) {
			gridPositionVec = cells->cells[scanningCell].p;
			gridPositionVec.x = gridPositionVec.x + scanStart - scanSize;
			gridPosition = scanSize;
			statusChanged = false;
			lastTime = director::currentTime;
		}

		gridPositionVec.x += gridVelocity;
		gridPosition -= gridVelocity;

		if(gridPosition <= 0.0) {
			gridPositionRLT = 0;
			status = scenes[currentScene]->flowControl();
			statusChanged = true;
		}
	}

	if(status == GRID || status == STILL || status == UNSCAN) {
		scenes[currentScene]->update();
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
	gridVelocity = csp->getf("Scan.gridVelocity");

	scanSize = csp->getf("Scan.scanSize");
	scanStart = csp->getf("Scan.scanStart");

	gridDeleteRadius = csp->getf("Scan.deleteRadius");
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