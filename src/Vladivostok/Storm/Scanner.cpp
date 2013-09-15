#include "Scanner.h"

Scanner::Scanner(CSParser *csp, vec3 upLeftNear, vec3 downRightFar, Cells *cells) {
	this->upLeftNear = upLeftNear;
	this->downRightFar = downRightFar;
	this->cells = cells;
	TRESHOLD_STOP = 2;
	TRESHOLD_NEXT = 2;
	LAST = 0;
	grid = new Grid(csp->getf("Scan.Grid.size"), csp->getf("Scan.Grid.divisions"));
	scanningCell = -1;

	gridShad = new Shader("Shaders/Vladivostok/stormScan.vert", "Shaders/Vladivostok/stormScan.frag");
	grid_M_Id = gridShad->getUniform("Model");
	grid_V_Id = gridShad->getUniform("View");
	grid_P_Id = gridShad->getUniform("Projection");
}

void Scanner::detect() {
	if(!cells->move) { //If we are on STOP
		if(director::currentTime-LAST > TRESHOLD_STOP) {
			cells->move = true; //HIT PLAY!
			LAST = director::currentTime;
		}
		return;
	}

	scanningCell = -1;

	for(int i=0; i<cells->cells.size(); i++) {
		if(inRange(cells->cells[i].p.x, upLeftNear.x, downRightFar.x) &&
		   inRange(cells->cells[i].p.y, upLeftNear.y, downRightFar.y) &&
		   inRange(cells->cells[i].p.z, upLeftNear.z, downRightFar.z)) {
			   if(scanningCell != -1) return;
			   else scanningCell = i;
		}
	}

	if(scanningCell != -1) {
		if(director::currentTime-LAST > TRESHOLD_NEXT) {
			cells->move = false;
			LAST = director::currentTime;
		}
	}
}

void Scanner::draw(mat4 *V, mat4 *P) {
	if(scanningCell != -1) {
		mat4 idd = translate(cells->cells[scanningCell].p);

		gridShad->use();
		glUniformMatrix4fv(grid_M_Id, 1, GL_FALSE, &idd[0][0]);
		glUniformMatrix4fv(grid_V_Id, 1, GL_FALSE, &(*V)[0][0]); 
		glUniformMatrix4fv(grid_P_Id, 1, GL_FALSE, &(*P)[0][0]); 
		grid->render();
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
	TRESHOLD_NEXT = csp->getf("Scan.InterStopsTime");
	TRESHOLD_STOP = csp->getf("Scan.StopTime");
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