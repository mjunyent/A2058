#pragma once
#ifndef VIDIAN_CONFREAD
#define VIDIAN_CONFREAD

#include "../Tobago/Tobago.h"
#include "../Trinidad/Trinidad.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

class ConfRead {
public:
	std::string filename;

	std::string MODE;
	int ResX, ResY;
	int WindowMode;
	vector<float> costs;
	THREEDMODES mode3d;
	glm::vec3 position, direction, up;
	float EyeSep, CenterDist;

	ConfRead(std::string name);

	void read();

};

#endif