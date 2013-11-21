#include "ModelTest.h"

ModelTest::ModelTest(Camera *cam) : Deferred(cam) {
	A3DS_model = new A3dsHandler("Models/flagella.3DS", 0);
	A3DS_model->makeNormalsPerVertex();
	A3DS_model->makeBoundingBox();

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
					  1.0/A3DS_model->maxDimension,
					  NULL,
					  NULL);

	M_model = glm::translate(-A3DS_model->center*model->scale);

	lights->addDirectionalLight(glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, 1.0, -1.0), glm::vec3(1.0, 1.0, 1.0));
}

void ModelTest::render(int s, double t) {
	model->render();
}
