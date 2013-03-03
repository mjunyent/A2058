#pragma once
#ifndef PRUEVA
#define PRUEVA

#include "Tobago/Tobago.h"
//#include "NewScena.h"

class NewScena : public Scene {
public:
	NewScena();
	void draw(double time);
	void update(double time);
private:
	Shader *theShad;
};

#endif