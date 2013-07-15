#pragma once
#ifndef TRINIDAD_LAUNCHSHADER
#define TRINIDAD_LAUNCHSHADER

#include "Tobago/Tobago.h"

class LaunchShader : public Scene {
public:
	Shader *shader;
	LaunchShader(char *vertex_file, char *fragment_file);
	void draw(double t);
	Shader* getShader();
};

#endif