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
	vec3 deflector;

	float K, M, L;

	Cells(int n, float v, float xRange, float yRange, float zNear, float zFar, float zFarAway, float K, float L, float M);

	void update();

	void readConf(CSParser *csp);
};