#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "Parser.h"

struct Cell
{
	vec3 p;
	vec3 v;
};

struct DepthSort
{
    bool operator()( const Cell& lx, const Cell& rx ) const {
    	return lx.p.z < rx.p.z;
    }
};

class Cells {
public:
	vector<Cell> cells;
	float xRange, yRange, zNear, zFar, zFarAway;
	float vel;
	bool move;
	vec3 deflector;

	float K, M, L;

	Cells(int n, float v, float xRange, float yRange, float zNear, float zFar, float zFarAway, float K, float L, float M);

	void update();

	void readConf(CSParser *csp);
};

class Scanner {
public:
	vec3 upLeftNear, downRightFar;
	float LAST; //last stop time
	float TRESHOLD_STOP; //time in stop
	float TRESHOLD_NEXT; //time to wait before next stop
	Cells *cells;

	Shader *debShad;
	VBO *debBox;
	
	Scanner(vec3 upLeftNear, vec3 downRightFar, Cells *cells);

	void debSetup();
	void renderDebugBox(glm::mat4 *M, glm::mat4 *V, glm::mat4 *P);
	

	void detect();
	void draw();

	void readConf(CSParser *csp);

	GLint deb_M_Id, deb_V_Id, deb_P_Id, deb_Color_Id;
};