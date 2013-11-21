#include "Cilinder.h"

Cilinder::Cilinder() : Deferred() {
	int width, height;
	glfwGetWindowSize(director::windows[0], &width, &height);

	myCam = new Camera(width, height,
		   45.0f, 2.0, 160.0,
		   glm::vec3(-25.0f, 18.0f, -25.0f), glm::vec3(1.0f, -0.5f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		   14.0, 28.1f, 2.5f);

	myRig = new Rig(*myCam, 0.2);

	setup(myRig);

	cilinder3DS = new A3dsHandler("Models/cilinder.3DS", 0);
	cilinder3DS->makeNormalsPerVertex();

	M_cilinder = glm::translate(0,0,0);

	cilinder_model = new Model(firstShad,
							   cilinder3DS->vertexs,
							   cilinder3DS->normals,
							   NULL,
							   NULL,
							   NULL,
							   cilinder3DS->indexs,
							   0.4,
							   glm::vec3(0.4f, 0.4f, 0.4f),
							   glm::vec3(1.0f, 1.0f, 1.0f),
							   0.2f,
							   &M_cilinder,
							   1.0,
							   NULL,
							   NULL);

	//lights->addSpotLight(glm::vec3(0.0, 20.0, 0.0), glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.80, 0.96, 2.0);
	lights->addDirectionalLight(glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, 1.0, -1.0), glm::vec3(1.0, 1.0, 1.0));

	dotheDOF();
}

void Cilinder::render(int s, double t) {
	M_cilinder = glm::translate(0,0,0);
	cilinder_model->render();
	M_cilinder = glm::translate(4,0,0);
	cilinder_model->render();
	M_cilinder = glm::translate(-4,0,0);
	cilinder_model->render();
}

void Cilinder::update(double t) {
  try {
    read_info("Config/Cilinder.txt", properties);
  }
  catch (boost::property_tree::info_parser_error e) {
	  TOBAGO::log.write(WARNING) << "Could not read config file SimpleCells";
  }

  this->cam->fov = properties.get<float>("Camera.fov");
  this->cam->znear = properties.get<float>("Camera.znear");
  this->cam->zfar = properties.get<float>("Camera.zfar");

  this->cam->position[0] = properties.get<float>("Camera.px");
  this->cam->position[1] = properties.get<float>("Camera.py");
  this->cam->position[2] = properties.get<float>("Camera.pz");

  this->cam->direction[0] = properties.get<float>("Camera.dx");
  this->cam->direction[1] = properties.get<float>("Camera.dy");
  this->cam->direction[2] = properties.get<float>("Camera.dz");

  this->cam->up[0] = properties.get<float>("Camera.upx");
  this->cam->up[1] = properties.get<float>("Camera.upy");
  this->cam->up[2] = properties.get<float>("Camera.upz");

  this->cam->focalLength = properties.get<float>("Lens.fl");
  this->cam->focusDistance = properties.get<float>("Lens.fd");
  this->cam->FStop = properties.get<float>("Lens.fs");

  if(rig != NULL)
	  this->rig->eye_sep = properties.get<float>("Rig.eyesep");

  this->cam->update(t);
  if(rig != NULL) this->rig->update(t);
}