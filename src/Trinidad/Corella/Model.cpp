#include "Model.h"

Model::Model(Shader *shader,
			 VBO *vertexs,
			 VBO *normals,
			 IBO *indexs,
			 float ambient_factor,
			 vec3 diffuse_color,
			 vec3 specular_color,
			 float shininess,
			 mat4 *M,
			 float scale)
{
	this->shader = shader;
	this->vertexs = vertexs;
	this->normals = normals;
	this->indexs = indexs;

	this->ambient_factor = ambient_factor;
	this->diffuse_color = diffuse_color;
	this->diffuse_texture = NULL;
	this->specular_color = specular_color;

	this->shininess = shininess;

	this->M = M;
	this->scale = scale;

	shininess_id = shader->getUniform("shininess");
	scale_id = shader->getUniform("scale");
	M_id = shader->getUniform("Model");
	ambient_factor_id = shader->getUniform("AmbientFactor");
	diffuse_color_id = shader->getUniform("DiffuseColor");
	specular_color_id = shader->getUniform("SpecularColor");
}

void Model::render() {
	glUniformMatrix4fv(M_id, 1, GL_FALSE, &(*M)[0][0]);
	glUniform1f(scale_id, scale);
	glUniform1f(shininess_id, shininess);
	glUniform1f(ambient_factor_id, ambient_factor);
	glUniform3fv(diffuse_color_id, 1, &diffuse_color[0]);
	glUniform3fv(specular_color_id, 1, &specular_color[0]);

	vertexs->enable(3);
	normals->enable(3);
	indexs->draw(GL_TRIANGLES);
	vertexs->disable();
	normals->disable();
}