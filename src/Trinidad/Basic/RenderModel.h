#pragma once
#ifndef TRINIDAD_RENDERMODEL
#define TRINIDAD_RENDERMODEL

#include "Tobago/Tobago.h"

/**
	Prerequisites:
		-shader must have: layout0 = vec3 for Vertexs.
						   layout1 = vec3 for Normals.
		-Uniform Mat4 named MVP to allocate the MVP matrix.
*/

class RenderModel : public Scene {
public:
	Shader *shader;
	A3dsHandler *model;
	glm::mat4 *MVP;
	GLuint MVP_Id;

	RenderModel(char* vertex_file, char* fragment_file, char* model_file, int meshId, glm::mat4* MVP);
	void draw(double t);
};

#endif