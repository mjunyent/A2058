#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "../Parser.h"
#include "Cells.h"
#include "Models.h"
#include "Scenes/First.h"

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

	//Parameters.
	float restTime, startTime, gridVelocity, stillTime;
	float scanSize, scanStart, scanTextStart, gridDeleteRadius;
	vec3 upLeftNear, downRightFar;
	float distanceFade;

	//State Variables
	STATE status;
	int scanningCell;		 //Current cell in scan.
	float lastTime;			 //Last time a Status change happened.
	vec3 gridPositionVec;    //Current grid position 3D space.
	float gridPosition;		 //Current grid position [0, scanSize]
	Cells *cells;			 //All cells status.

	//Render things.
	Rig *rig;
	Shader *gridShad;
	Shader *mixShad;
	Shader *textShad;
	Shader *debShad;
	VBO *debBox;
	VBO *quad, *textQuadCoords;
	IBO *quad_I;
	FBO *impas;
	Grid *grid;

	//Scenes
	FirstStormScene *first;


	Scanner(CSParser *csp, Cells *cells, Rig *rig);

	void debSetup();
	void renderDebugBox(glm::mat4 *M, glm::mat4 *V, glm::mat4 *P);

	void detect();
	float draw(mat4 *V, mat4 *P, FBO *render, bool left);
	void renderModel();
	void drawModel(mat4 *V, mat4 *P, FBO *render, bool left);
	void drawText(mat4 *V, mat4 *P, FBO *render);
	void drawGrid(mat4 *V, mat4 *P, FBO *render);

	float worldToClip(mat4 *V, mat4 *P, vec3 *v);

	void readConf(CSParser *csp);
	void update();

	GLint grid_M_Id, grid_V_Id, grid_P_Id, grid_centerPosition_Id, grid_radius_Id, grid_alpha_Id;
	GLint mix_position_Id, mix_Tex_Id, mix_Depth_Id;
	GLint text_M_Id, text_V_Id, text_P_Id, text_sP_Id, text_image_Id;
	GLint deb_M_Id, deb_V_Id, deb_P_Id, deb_Color_Id;
};
