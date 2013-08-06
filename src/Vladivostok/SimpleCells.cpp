#include "SimpleCells.h"

MyCells::MyCells(int width, int height, glm::mat4 *P, glm::mat4 *V, glm::vec3 *cam_position) : Deferred(width, height, P, V, cam_position) {
	vessel = new A3dsHandler("Models/vessel.3ds", 1);
	vessel->calculateNormals();

	M_Vessel = glm::rotate(90.0f, glm::vec3(1, 0, 0));

	vessel_model = new Model(firstShad,
							 vessel->vertexs,
							 vessel->normals,
							 vessel->indexs,
							 0.2,
							 glm::vec3(156.0/255.0f, 9.0/255.0f, 9.0/255.0f),
							 glm::vec3(0.4f, 0.4f, 0.4f),
							 0.512f,
							 &M_Vessel,
							 1.0f);
};

void MyCells::update(double time) {
}

void MyCells::render(int s, double t) {
	vessel_model->render();
}