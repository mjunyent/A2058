#pragma once

#include "../Tobago.h"

using namespace glm;

class Model {
	public :
		Shader *shader;

		VBO *vertexs;
		VBO *normals;
		VBO *texCoords;

		IBO *indexs;

		TBO *diffuse_texture;

		vec3 ambient_color;
		vec3 diffuse_color;
		vec3 specular_color;
		float shininess;

		float scale;
		mat4 *M;

		GLint M_id, ambient_color_id, diffuse_color_id, specular_color_id, shininess_id, scale_id;

		Model(Shader *shader,				//Shader that will render the object.
			  VBO *vertexs,					//Vertex info.
			  VBO *normals,					//Normals info.
			  IBO *indexs,					//Faces indexs.
			  vec3 ambient_color,			//Ambient color.
			  vec3 diffuse_color,			//Difuse color.
			  vec3 specular_color,			//Specular color.
			  float shininess,				//Shininess factor.
			  mat4 *M,						//Model matrix.
			  float scale					//Object scale factor.
			 );

		void render();
};