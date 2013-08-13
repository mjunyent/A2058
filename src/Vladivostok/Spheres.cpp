#include "Spheres.h"

Spheres::Spheres(Camera *cam) : Deferred(cam) {
	sp = new Sphere(15.0, 20);
	fl = new Floor(200.0);
	vessel = new A3dsHandler("Models/sphere.3DS", 0);
	vessel->makeNormalsPerVertex();

	M_floor  = glm::translate(0,0,0);

	sphere_model = new Model(firstShad,
							 sp->vertexs,
							 sp->normals,
							 NULL,
							 NULL,
							 NULL,
							 sp->indexs,
							 0.2,
							 glm::vec3(156.0/255.0f, 9.0/255.0f, 9.0/255.0f),
							 glm::vec3(1.0f, 1.0f, 1.0f),
							 0.052f,
							 &M_sphere,
							 1.00,
							 NULL,
							 NULL);

	floor_model = new Model(firstShad,
							fl->vertexs,
							fl->normals,
							NULL,
							NULL,
							NULL,
							fl->indexs,
							0.2,
							glm::vec3(1.0, 1.0, 1.0),
							glm::vec3(1.0, 1.0, 1.0),
							0.128f,
							&M_floor,
							1.00,
							NULL,
							NULL);

			



	bool lecalite[] = { true };
}

void Spheres::render(int s, double t) {
	sphere_model->render();
	floor_model->render();
}

void Spheres::update(double t) {
	M_sphere = glm::translate(float(20.0f*sin(director::currentTime)), 15.0f, float(20.0f*cos(director::currentTime)));

	lights->lights[0].Direction = -lights->lights[0].Position + glm::vec3(float(20.0f*sin(director::currentTime)), 15.0f, float(20.0f*cos(director::currentTime)));
}