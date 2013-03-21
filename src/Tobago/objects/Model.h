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

		mat4 *M;

		Model(Shader *shader, VBO *vertexs, VBO *normals, IBO *indexs, vec3 ambient_color, vec3 diffuse_color, vec3 specular_color, float shininess, mat4 *M);

		void render();
};