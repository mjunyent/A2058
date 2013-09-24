#include "Model.h"

Model::Model(Shader *shader,
			  VBO *vertexs,
			  VBO *normals,
			  VBO *UVs,
  			  VBO *tangents,
			  VBO *bitangents,
			  IBO *indexs,
			  float ambient_factor,
			  vec3 diffuse_color,
			  vec3 specular_color,
			  float shininess,
			  mat4 *M,
			  float scale,
			  char* diffuse_texture,
			  char* bumpMap)
{
	this->shader = shader;

	this->vertexs = vertexs;
	this->normals = normals;
	this->UVs = UVs;
	this->tangents = tangents;
	this->bitangents = bitangents;
	this->indexs = indexs;

	if(diffuse_texture != NULL) {
		this->diffuse_texture = TBO(diffuse_texture, true);
		isTextured = true;
	} else isTextured = false;
	if(bumpMap != NULL) {
		this->bumpMap = TBO(bumpMap, true);
		isBump = true;
	}
	else isBump = false;

	this->ambient_factor = ambient_factor;
	this->diffuse_color = diffuse_color;
	this->specular_color = specular_color;
	this->shininess = shininess;

	this->scale = scale;
	this->M = M;

	shininess_id = shader->getUniform("shininess");
	scale_id = shader->getUniform("scale");
	M_id = shader->getUniform("Model");
	ambient_factor_id = shader->getUniform("AmbientFactor");
	diffuse_color_id = shader->getUniform("DiffuseColor");
	specular_color_id = shader->getUniform("SpecularColor");

	bumpMap_id = shader->getUniform("NormalTex");
	isBumpMap_id = shader->getUniform("isNormalTex");

	diffuse_texture_id = shader->getUniform("DiffuseTexture");
	isTexture_id = shader->getUniform("isTextured");
}

void Model::render() {
	glUniformMatrix4fv(M_id, 1, GL_FALSE, &(*M)[0][0]);
	glUniform1f(scale_id, scale);
	glUniform1f(shininess_id, shininess);
	glUniform1f(ambient_factor_id, ambient_factor);
	glUniform3fv(diffuse_color_id, 1, &diffuse_color[0]);
	glUniform3fv(specular_color_id, 1, &specular_color[0]);

	if(!isBump) glUniform1i(isBumpMap_id, 0);
	else {
		glUniform1i(isBumpMap_id, 1);
		bumpMap.bind(0);
		glUniform1i(bumpMap_id, 0);
	}

	if(!isTextured) glUniform1i(isTexture_id, 0);
	else {
		glUniform1i(isTexture_id, 1);
		diffuse_texture.bind(1);
		glUniform1i(diffuse_texture_id, 1);
	}

	vertexs->enable(3);
	if(normals != NULL) normals->enable(3);
	if(UVs != NULL) UVs->enable(2);
	if(tangents != NULL) tangents->enable(3);
	if(bitangents != NULL) bitangents->enable(3);
	indexs->draw(GL_TRIANGLES);
	if(bitangents != NULL) bitangents->disable();
	if(tangents != NULL) tangents->disable();
	if(UVs != NULL) UVs->disable();
	if(normals != NULL) normals->disable();
	vertexs->disable();
}