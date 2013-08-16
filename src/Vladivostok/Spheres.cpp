#include "Spheres.h"

Spheres::Spheres(Camera *cam) : Deferred(cam) {
	sp = new Sphere(1.0, 20);
	fl = new Floor(20.0);

	int width, height;
	glfwGetWindowSize(director::windows[0], &width, &height);

/*	this->cam = new Camera(width, height,
						   45.0f, 1.0, 50.0,
						   glm::vec3(25.0f, 8.0f, 25.0f), glm::vec3(-1.0f, -0.5f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
						   12.5, 17.0, 4.5f);
						   */
	M_floor  = glm::translate(0,0,0);

	sphere_model = new Model(firstShad,
							 sp->vertexs,
							 sp->normals,
							 NULL,
							 NULL,
							 NULL,
							 sp->indexs,
							 0.2,
							 glm::vec3(0.5, 0.5, 0.5),
//							 glm::vec3(156.0/255.0f, 9.0/255.0f, 9.0/255.0f),
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
							0.4,
							glm::vec3(1.0, 1.0, 1.0),
							glm::vec3(1.0, 1.0, 1.0),
							0.128f,
							&M_floor,
							1.00,
							NULL,
							NULL);


	lights->addSpotLight(glm::vec3(0.0, 20.0, 0.0), glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.80, 0.96, 2.0);
	lights->addDirectionalLight(glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, 1.0, -1.0), glm::vec3(1.0, 1.0, 1.0));

	dotheDOF();
}

void Spheres::render(int s, double t) {
	floor_model->render();

	M_sphere = glm::translate(-20.0f, 1.0f, -20.0f);

	for(int i=0; i<9; i++) {
		M_sphere = M_sphere*glm::translate(4.0f, 0.0f, 0.0f);
		for(int j=0; j<9; j++) {
			M_sphere = M_sphere*glm::translate(0.0f, 0.0f, 4.0f);
			if(i != 4 || j != 4) sphere_model->render();
		}
		M_sphere = M_sphere*glm::translate(0.0f, 0.0f, -36.0f);
	}
}

void Spheres::update(double t) {

}