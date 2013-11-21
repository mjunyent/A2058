#include "Models.h"

Models::Models(Camera *cam) : Deferred(cam) {
	A3DS_model = new A3dsHandler("Models/flagella.3DS", 0);
	A3DS_model->makeNormalsPerVertex();
	A3DS_model->makeBoundingBox();

	M_model = glm::translate(-A3DS_model->center);

	model = new Model(firstShad,
					  A3DS_model->vertexs,
					  A3DS_model->normals,
					  NULL,
					  NULL,
					  NULL,
					  A3DS_model->indexs,
					  0.4,
					  glm::vec3(0.4f, 0.4f, 0.4f),
					  glm::vec3(1.0f, 1.0f, 1.0f),
					  0.2f,
					  &M_model,
					  50.0/A3DS_model->maxDimension,
					  NULL,
					  NULL);

	lights->addDirectionalLight(glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, 1.0, -1.0), glm::vec3(1.0, 1.0, 1.0));
}

void Models::translate(glm::vec3 here) {
	M_model = glm::translate(here) * glm::translate(-A3DS_model->center*model->scale);
	vec4 wtf = M_model * vec4(A3DS_model->center, 1.0);
}

void Models::render(int s, double t) {
	model->render();
}
