#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "../Parser.h"
#include "Cells.h"
#include "Models.h"
#include "Scenes/StormScene.h";
#include "Scenes/First.h"
#include "Scenes/Second.h"
#include "Scenes/Third.h"
#include "Scenes/Fourth.h"
#include "Scenes/Fifth.h"
#include "Scenes/SixSeventh.h"
#include "Scenes/Eight.h"
#include "Scenes/Ninth.h"
#include "Scenes/Tenth.h"

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
	//Parameters.
	float restTime, startTime, gridVelocity;
	float scanSize, scanStart, gridDeleteRadius;
	vec3 upLeftNear, downRightFar;
	float distanceFade;

	//State Variables
	STATE status;
	bool statusChanged;
	int scanningCell;		 //Current cell in scan.
	float lastTime;			 //Last time a Status change happened.
	vec3 gridPositionVec;    //Current grid position 3D space.
	float gridPosition;		 //Current grid position [0, scanSize]
	Cells *cells;			 //All cells status.
	int side; //0 draw all, 1 show right, 2 show left, 3 show none.
	int currentScene;
	int gridPositionRLT; //0 right, 1 left, 2 traveling.

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
	FBO *renderBufferL, *renderBufferR;
	Grid *grid;

	TBO linesCircleLeft, linesCircleRight, linesQuadLeft, linesQuadRight;

	//Scenes
	vector<StormScene*> scenes;

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
	GLint mix_position_Id, mix_TexL_Id, mix_TexR_Id, mix_DepthL_Id, mix_DepthR_Id, mix_showL_Id, mix_showR_Id;
	GLint text_M_Id, text_V_Id, text_P_Id, text_sP_Id, text_leftTex_Id, text_rightTex_Id, text_showL_Id, text_showR_Id;
	GLint deb_M_Id, deb_V_Id, deb_P_Id, deb_Color_Id;
};
