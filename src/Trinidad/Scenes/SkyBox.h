#pragma once

#include "Tobago/Tobago.h"

using namespace std;
using namespace glm;

class SkyBox {
	public :
		VBO *vertexs;		//vertex VBO
		VBO *normals;		//Normals VBO
		VBO *UVs;		//Texture coordinates VBO
//		VBO *tangents;
//		VBO *bitangents;
		IBO *indexs;		//Indexs...

//		TBO *diffuse_texture;	//Texture for diffuse
//		TBO bumpMap;

		SkyBox(float radius);
};