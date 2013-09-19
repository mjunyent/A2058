#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "../Parser.h"

class Models : public Deferred {
public:
	A3dsHandler *A3DS_model;
	Model *model;
	glm::mat4 M_model;

	Models(Camera *cam);

	void translate(glm::vec3 here);

	void render(int s, double t);

	void update();
};