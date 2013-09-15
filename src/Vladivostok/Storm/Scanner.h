#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "../Parser.h"
#include "Cells.h"

class Grid {
public:
	Grid(float size, int divisions);

	void render();

	VBO *lines;
private:
	vector<float> lineline;
	void pushVertex(float x, float y, float z);
};


class Scanner {
public:
	vec3 upLeftNear, downRightFar;
	float LAST; //last stop time
	float TRESHOLD_STOP; //time in stop
	float TRESHOLD_NEXT; //time to wait before next stop
	Cells *cells;
	int scanningCell;

	Shader *gridShad;
	Shader *debShad;
	VBO *debBox;
	Grid *grid;

	
	Scanner(CSParser *csp, vec3 upLeftNear, vec3 downRightFar, Cells *cells);

	void debSetup();
	void renderDebugBox(glm::mat4 *M, glm::mat4 *V, glm::mat4 *P);

	void detect();
	void draw(mat4 *V, mat4 *P);

	void readConf(CSParser *csp);

private:
	GLint grid_M_Id, grid_V_Id, grid_P_Id;
	GLint deb_M_Id, deb_V_Id, deb_P_Id, deb_Color_Id;
};
