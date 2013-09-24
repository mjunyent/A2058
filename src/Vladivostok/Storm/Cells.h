#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "../Parser.h"

struct Cell
{
	int id;
	vec3 p;
	vec3 v;
	float alpha;
};

struct DepthSort
{
    bool operator()( const Cell* lx, const Cell* rx ) const {
    	return lx->p.z < rx->p.z;
    }
};

class Cells {
public:
	vector<Cell> cells;
	vector<Cell*> sortedCells;
	float xRange, yRange, zNear, zFar, zFarAway;
	float vel;
	bool move;
	bool reduce;
	int play;
	int reduceId;
	float reduceMult, reduceVel;
	float alphaVel;
	int selectedCell;
	float centerVelocity, centerUncertainty;
	vec3 centerPosition;
	float slowGo;

	float MultMax, MultIncr, MultDecr;

	vec3 deflector;
	float deflectorL, deflectorM;

	float H, K, M, L;

	Cells(int n, CSParser *csp);

	void slowStop(int cell);
	void update();
	void Play();
	void select(int cell);

	void readConf(CSParser *csp);
};