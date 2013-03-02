#include "LaunchShader.h"

LaunchShader::LaunchShader(char *vertex_file, char *fragment_file) {
	shader = new Shader(vertex_file, fragment_file);
}

void LaunchShader::draw(double time) {
	shader->use();
}

Shader* LaunchShader::getShader() {
	return shader;
}