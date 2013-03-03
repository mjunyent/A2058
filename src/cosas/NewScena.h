#pragma once
#ifndef PRUEVA
#define PRUEVA

#include "Tobago/Tobago.h"

class NewScena : public Scene {
public:
	NewScena(glm::mat4* MVP);
	void draw(double time);
	void update(double time);
	glm::mat4 *MVP;
	GLuint MVP_Id;
private:
	Shader *pint;
	
};

#endif