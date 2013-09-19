#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "../Parser.h"
#include "Cells.h"
#include "Models.h"

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
				 DETECTING,  //cells moving, waiting for something to happen.
				 START,		 //cell in range detected, starting scan. Everything Pauses, no grid.
				 GRID,		 //Grid Movement Pattern.
				 STILL,		 //Rendered scan for a few seconds.
				 UNSCAN };	 //Move Back Grid.

	STATE status;
	FBO *impas;
	float lastTime;
	float restTime, startTime, gridVelocity, stillTime;
	int scanningCell;
	float scanSize, scanStart, scanTextStart;
	vec3 upLeftNear, downRightFar;
	vec3 gridPositionVec;
	float gridPosition, gridDeleteRadius;
	float distanceFade;
	Models *scanned;
	TBO text;

	Cells *cells;

	Shader *gridShad;
	Shader *mixShad;
	Shader *textShad;
	Shader *debShad;
	VBO *debBox;
	VBO *quad, *textQuad, *textQuadCoords;
	IBO *quad_I;
	Grid *grid;

	Scanner(CSParser *csp, Cells *cells, Rig *rig);

	void debSetup();
	void renderDebugBox(glm::mat4 *M, glm::mat4 *V, glm::mat4 *P);

	void detect();
	float draw(mat4 *V, mat4 *P, FBO *render, bool left);
	void renderModel();
	void drawModel(mat4 *V, mat4 *P, FBO *render, bool left);
	void drawText(mat4 *V, mat4 *P, FBO *render);
	void drawGrid(mat4 *V, mat4 *P, FBO *render);

	void readConf(CSParser *csp);
	void update();

private:
	GLint grid_M_Id, grid_V_Id, grid_P_Id, grid_centerPosition_Id, grid_radius_Id, grid_alpha_Id;
	GLint mix_position_Id, mix_Tex_Id, mix_Depth_Id;
	GLint text_M_Id, text_V_Id, text_P_Id, text_sP_Id, text_image_Id;
	GLint deb_M_Id, deb_V_Id, deb_P_Id, deb_Color_Id;
};
