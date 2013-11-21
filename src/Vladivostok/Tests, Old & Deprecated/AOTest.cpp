#include "AOTest.h"

AOTest::AOTest(int Screen) : Deferred() {
	this->Screen = Screen;
	glfwMakeContextCurrent(director::windows[Screen]);

	int width, height;
	glfwGetWindowSize(director::windows[0], &width, &height);

	myCam = new Camera(width, height,
		   45.0f, 2.0, 160.0,
		   glm::vec3(-25.0f, 18.0f, -25.0f), glm::vec3(1.0f, -0.5f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		   14.0, 28.1f, 2.5f);

	myRig = new Rig(*myCam, 0.2);

	setup(myRig);

	M_world = glm::translate(0,0,0);

	box = new SkyBox(10.0);
	M_box = glm::translate(0,0,0);
	box_model = new Model(firstShad,
						  box->vertexs,
						  box->normals,
						  NULL,
						  NULL,
						  NULL,
						  box->indexs,
						  0.4,
						  glm::vec3(1,1,1),
						  glm::vec3(1,1,1),
						  0.512f,
						  &M_box,
						  1.0,
						  NULL,
						  NULL);

	sphere = new Sphere(0.5, 20);
	M_sphere = glm::translate(0.0f, -9.5f, 0.0f);
	sphere_model = new Model(firstShad,
							 sphere->vertexs,
							 sphere->normals,
							 NULL,
							 NULL,
							 NULL,
							 sphere->indexs,
							 0.4,
							 glm::vec3(1, 1, 1),
							 glm::vec3(1, 1, 1),
							 0.512f,
							 &M_sphere,
							 1.0,
							 NULL,
							 NULL);

	dotheAO(30, 0.05, glm::vec2(1, 5));

	//lights->addSpotLight(glm::vec3(0.0, 20.0, 0.0), glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.80, 0.96, 2.0);
//	lights->addDirectionalLight(glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, 1.0, -1.0), glm::vec3(1.0, 1.0, 1.0));
	lights->addPointLight(glm::vec3(0.0, 0.0, 0.0), glm::vec3(2.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
//	lights->addDirectionalLight(glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
	
	dotheDOF();
//	renderOfscreen(false);
}

void AOTest::draw(int s, double t) {
	if(s == Screen) Deferred::draw(s, t);
}

void AOTest::render(int s, double t) {
	TOBAGO::log.write(DEBUG) << "Screen: " << s;
	if(s == 0) M_box = glm::rotate(20.0f, glm::vec3(0.0, 1.0, 0.0))*M_world;
	if(s == 1) M_box = glm::rotate(-20.0f, glm::vec3(1.0, 0.0, 0.0))*M_world;
	if(s == 2) M_box = glm::rotate(20.0f, glm::vec3(0.0, 0.0, 1.0))*M_world;
	//Box.
	box_model->render();

	//First floor spheres.
	M_sphere = M_world*glm::translate(0.0f, -9.5f, 0.0f);
	sphere_model->render();

	M_sphere = M_world*glm::translate(1.0f, -9.5f, 0.0f);
	sphere_model->render();
	M_sphere = M_world*glm::translate(-1.0f, -9.5f, 0.0f);
	sphere_model->render();
	M_sphere = M_world*glm::translate(0.0f, -9.5f, 1.0f);
	sphere_model->render();
	M_sphere = M_world*glm::translate(0.0f, -9.5f, -1.0f);
	sphere_model->render();

	M_sphere = M_world*glm::translate(1.0f, -9.5f, 1.0f);
	sphere_model->render();
	M_sphere = M_world*glm::translate(-1.0f, -9.5f, 1.0f);
	sphere_model->render();
	M_sphere = M_world*glm::translate(1.0f, -9.5f, -1.0f);
	sphere_model->render();
	M_sphere = M_world*glm::translate(-1.0f, -9.5f, -1.0f);
	sphere_model->render();

	//Second Floor.
	M_sphere = glm::translate(0.5f, -8.79289f, 0.5f)*M_world;
	sphere_model->render();
	M_sphere = glm::translate(0.5f, -8.79289f, -0.5f)*M_world;
	sphere_model->render();
	M_sphere = glm::translate(-0.5f, -8.79289f, 0.5f)*M_world;
	sphere_model->render();
	M_sphere = glm::translate(-0.5f, -8.79289f, -0.5f)*M_world;
	sphere_model->render();

	//Third Floor.
	M_sphere = glm::translate(0.0f, -8.085786f, -0.0f)*M_world;
	sphere_model->render();
}

void AOTest::update(double t) {
	M_world *= glm::rotate(0.1f, glm::vec3(0.0f, 1.0f, 0.0f));

  try {
    read_info("Config/AOTest.txt", properties);
  }
  catch (boost::property_tree::info_parser_error e) {
	  TOBAGO::log.write(WARNING) << "Could not read config file SimpleCells";
  }


  box_model->scale = properties.get<float>("Box.scale");
  box_model->shininess = properties.get<float>("Box.shininess");

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

  this->AO_bias = properties.get<float>("Occlusion.bias");
  this->AO_radius = properties.get<float>("Occlusion.radius");
  this->AO_attenuation[0] = properties.get<float>("Occlusion.linearAtt");
  this->AO_attenuation[1] = properties.get<float>("Occlusion.quadraticAtt");

  if(rig != NULL)
	  this->rig->eye_sep = properties.get<float>("Rig.eyesep");

  this->cam->update(t);
  if(rig != NULL) this->rig->update(t);
}