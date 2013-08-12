#include "Spheres.h"

Spheres::Spheres(Camera *cam) : Deferred(cam) {
	sp = new Sphere(50.0, 20);

	vessel = new A3dsHandler("Models/sphere.3DS", 0);
	vessel->makeNormalsPerVertex();

	M_Vessel = glm::translate(0,0,0);

	vessel_model = new Model(firstShad,
		sp->vertexs,
		sp->normals,
//							 vessel->vertexs,
//							 vessel->normals,
							 NULL,
							 NULL,
							 NULL,
							 sp->indexs,
//							 vessel->indexs,
							 0.2,
							 glm::vec3(156.0/255.0f, 9.0/255.0f, 9.0/255.0f),
							 glm::vec3(0.4f, 0.4f, 0.4f),
							 0.512f,
							 &M_Vessel,
							 1.01,
							 NULL,
							 NULL);

	bool lecalite[] = { true };
}

void Spheres::render(int s, double t) {
	vessel_model->render();
}