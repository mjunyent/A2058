#include "Spheres.h"

Spheres::Spheres() : Deferred() {
	int width, height;
	glfwGetWindowSize(director::windows[0], &width, &height);

	myCam = new Camera(width, height,
		   45.0f, 2.0, 160.0,
		   glm::vec3(-25.0f, 18.0f, -25.0f), glm::vec3(1.0f, -0.5f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		   14.0, 28.1f, 2.5f);

	myRig = new Rig(*myCam, 0.2);

	firstShad = new Shader("Shaders/Vladivostok/SpheresFirst.vert", "Shaders/Vladivostok/SpheresFirst.frag"); 
	emissiveID = firstShad->getUniform("emissive");

	bool calite[] = { true, true, true, true };
	renderBufferL = new FBO(myCam->width, myCam->height, true, 4, calite);
	renderBufferR = new FBO(myCam->width, myCam->height, true, 4, calite);

	setup(myRig);

	sp = new Sphere(1.0, 20);
	fl = new Floor(120.0);

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
							0.2,
							glm::vec3(1.0, 1.0, 1.0),
							glm::vec3(1.0, 1.0, 1.0),
							0.128f,
							&M_floor,
							1.00,
							NULL,
							NULL);


	lights->addSpotLight(glm::vec3(0.0, 20.0, 0.0), glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.80, 0.96, 2.0);
	lights->addDirectionalLight(glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, 1.0, -1.0), glm::vec3(1.0, 1.0, 1.0));

	//dotheDOF();
}

void Spheres::render(int s, double t) {
	bool emits = false;
	glUniform1f(emissiveID, 0.0);
	//floor_model->render();

	M_sphere = glm::translate(-20.0f, 1.0f, -20.0f);

	for(int i=0; i<19; i++) {
		M_sphere = M_sphere*glm::translate(7.0f, 0.0f, 0.0f);
		for(int j=0; j<19; j++) {
			if(emits) {
				glUniform1f(emissiveID, 1.0);
//				sphere_model->diffuse_color = glm::vec3(9.0/255.0f, 156.0/255.0f, 9.0/255.0f);
			} else {
				glUniform1f(emissiveID, 0.0);
	//			sphere_model->diffuse_color = glm::vec3(0.5, 0.5, 0.5);
			}

			M_sphere = M_sphere*glm::translate(0.0f, 0.0f, 7.0f);
			if(i != 4 || j != 4) sphere_model->render();
			emits = !emits;
		}
		M_sphere = M_sphere*glm::translate(0.0f, 0.0f, -19*7.0f);
	}
}

void Spheres::update(double t) {
  try {
    read_info("Config/Spheres.txt", properties);
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