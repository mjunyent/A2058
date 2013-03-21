#include "Model.h"

Model::Model(Shader *shader, VBO *vertexs, VBO *normals, IBO *indexs, vec3 ambient_color, vec3 diffuse_color, vec3 specular_color, float shininess, mat4 *M) {
	this->shader = shader;
	this->vertexs = vertexs;
	this->normals = normals;
	this->indexs = indexs;

	this->ambient_color = ambient_color;
	this->diffuse_color = diffuse_color;
	this->diffuse_texture = NULL;
	this->specular_color = specular_color;

	this->shininess = shininess;

	this->M = M;
}

void Model::render() {

}