#pragma once

#include "Tobago/Tobago.h"

using namespace glm;

class Model {
	public :
		Shader *shader;		//shader that renders the model

		VBO *vertexs;		//vertex VBO
		VBO *normals;		//Normals VBO
		VBO *UVs;		//Texture coordinates VBO
		VBO *tangents;
		VBO *bitangents;
		IBO *indexs;		//Indexs...

		TBO diffuse_texture;	//Texture for diffuse
		TBO bumpMap;

		float ambient_factor;	//Color parameters.
		vec3 diffuse_color;
		vec3 specular_color;
		float shininess;

		float scale;
		mat4 *M;				//Model matrix (pointer)

		bool isBump, isTextured;

		GLint M_id, ambient_factor_id, diffuse_color_id, specular_color_id, shininess_id, scale_id, diffuse_texture_id,
			  bumpMap_id, isTexture_id, isBumpMap_id;

		Model(Shader *shader,				//Shader that will render the object.
			  VBO *vertexs,					//Vertex info.
			  VBO *normals,					//Normals info.
			  VBO *UVs,						//Texture coordinates VBO.
			  VBO *tangents,
			  VBO *bitangents,
			  IBO *indexs,					//Faces indexs.
			  float ambient_factor,			//Ambient color.
			  vec3 diffuse_color,			//Difuse color.
			  vec3 specular_color,			//Specular color.
			  float shininess,				//Shininess factor.
			  mat4 *M,						//Model matrix.
			  float scale,					//Object scale factor.
			  char* diffuse_texture,		//Texture object.
			  char* bumpMap					//Bump map.
			  );

		void render();
};