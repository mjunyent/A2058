#include "SimpleCells.h"

MyCells::MyCells(int width, int height, glm::mat4 *P, glm::mat4 *V, glm::vec3 *cam_position) : Deferred(width, height, P, V, cam_position, 1) {
	vessel = new A3dsHandler("Models/sphere.3DS", 0);
	vessel->makeNormalsPerVertex();
	vessel->makeUVs();
	vessel->makeTBNSpace();

	M_Vessel = glm::translate(0, 0, 0);
//	M_Vessel = vessel->getModelMatrix();// glm:: glm::rotate(1.0f, glm::vec3(1, 0, 0));

	vessel_model = new Model(firstShad,
							 vessel->vertexs,
							 vessel->normals,
							 vessel->UVs,
							 vessel->tangents,
							 vessel->bitangents,
							 vessel->indexs,
							 0.2,
							 glm::vec3(156.0/255.0f, 9.0/255.0f, 9.0/255.0f),
							 glm::vec3(0.4f, 0.4f, 0.4f),
							 0.512f,
							 &M_Vessel,
							 1.01,
							 NULL,
							 "Images/normalesguarras.png");
}

void MyCells::update(double time) {
}

void MyCells::render(int s, double t) {
	vessel_model->render();
}