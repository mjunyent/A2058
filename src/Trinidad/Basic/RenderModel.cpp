#include "RenderModel.h"


RenderModel::RenderModel(char* vertex_file, char* fragment_file, char* model_file, int meshId, glm::mat4* MVP) {
	this->MVP = MVP;

	shader = new Shader(vertex_file, fragment_file);
	MVP_Id = shader->getUniform("MVP");

	model = new A3dsHandler(model_file);
	model->makeVBO(meshId);
	model->makeNormals();
}

void RenderModel::draw(double t) {
	shader->use();
	glUniformMatrix4fv(MVP_Id, 1, GL_FALSE, &(*MVP)[0][0]);

	model->vertexs->enable(3);
	model->normals->enable(3);
	model->vertexs->draw(GL_TRIANGLES);
	model->vertexs->disable();
	model->normals->disable();
}
