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
	enum STATE { REST,		 //cells moving, not scanning.
				 DETECTING,   //cells moving, waiting for something to happen.
				 START,		 //cell in range detected, starting scan. Everything Pauses, no grid.
				 GRID,		 //Grid Movement Pattern.
				 STILL };	 //Rendered scan for a few seconds.

	STATE status;
	FBO *impas;
	float lastTime;
	float restTime, startTime, gridVelocity, stillTime;
	int scanningCell;
	float scanSize, scanStart;
	vec3 upLeftNear, downRightFar;
	vec3 gridPositionVec;
	float gridPosition, gridDeleteRadius;

	Cells *cells;

	Shader *gridShad;
	Shader *debShad;
	VBO *debBox;
	Grid *grid;

	Scanner(CSParser *csp, Cells *cells);

	void debSetup();
	void renderDebugBox(glm::mat4 *M, glm::mat4 *V, glm::mat4 *P);

	void detect();
	void draw(mat4 *V, mat4 *P, FBO *render);

	void readConf(CSParser *csp);
	void update();

private:
	GLint grid_M_Id, grid_V_Id, grid_P_Id, grid_centerPosition_Id, grid_radius_Id;
	GLint deb_M_Id, deb_V_Id, deb_P_Id, deb_Color_Id;
};
