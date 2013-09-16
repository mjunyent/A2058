#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "../Parser.h"

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
	bool reduce;
	float reduceMult, reduceVel;

	vec3 deflector;

	float K, M, L;

	Cells(int n, CSParser *csp);

	void slowStop();
	void update();
	void Play();

	void readConf(CSParser *csp);
};