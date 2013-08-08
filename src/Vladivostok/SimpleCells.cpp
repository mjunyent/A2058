#include "SimpleCells.h"

MyCells::MyCells(int width, int height, glm::mat4 *P, glm::mat4 *V, glm::vec3 *cam_position) : Deferred(width, height, P, V, cam_position, 1) {
	vessel = new A3dsHandler("Models/flagella.3DS", 0);
//	vessel = new A3dsHandler("Models/sphere.3DS", 0);
	vessel->makeNormalsPerVertex();
//	vessel->makeFuckingNormals();
	vessel->makeUVs();
	vessel->makeTBNSpace();

	M_Vessel = glm::translate(14.8, 654.9, 0.0);
//	M_Vessel = glm::translate(0,0,0);
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
							 "Images/flagella.png");
}

void MyCells::update(double time) {
  try {
    read_info("Config/SimpleCells.txt", properties);
  }
  catch (boost::property_tree::info_parser_error e) {
	  TOBAGO::log.write(WARNING) << "Could not read config file SimpleCells";
  }

  float fov, ratio, znear, zfar;
  
  fov	= properties.get<float>("Perspective.fov");
  ratio	= properties.get<float>("Perspective.ratio");
  znear = properties.get<float>("Perspective.znear");
  zfar	= properties.get<float>("Perspective.zfar");

  *P = glm::perspective(fov, ratio, znear, zfar);

  glm::vec3 pos, dir, up, target;

  pos.x = properties.get<float>("Camera.px");
  pos.y = properties.get<float>("Camera.py");
  pos.z = properties.get<float>("Camera.pz");
  
  target.x = properties.get<float>("Camera.tx");
  target.y = properties.get<float>("Camera.ty");
  target.z = properties.get<float>("Camera.tz");

  up.x = properties.get<float>("Camera.ux");
  up.y = properties.get<float>("Camera.uy");
  up.z = properties.get<float>("Camera.uz");

  *V = glm::lookAt(pos, target, up);

  glm::vec3 translate1, rotate1;
  float angle1;

  translate1.x = properties.get<float>("Virus1.tx");
  translate1.y = properties.get<float>("Virus1.ty");
  translate1.z = properties.get<float>("Virus1.tz");

  rotate1.x = properties.get<float>("Virus1.rx");
  rotate1.y = properties.get<float>("Virus1.ry");
  rotate1.z = properties.get<float>("Virus1.rz");

  angle1 = properties.get<float>("Virus1.angle");

  M_Vessel = glm::translate(translate1)*glm::rotate(angle1, rotate1);

  translate1.x = properties.get<float>("Virus2.tx");
  translate1.y = properties.get<float>("Virus2.ty");
  translate1.z = properties.get<float>("Virus2.tz");

  rotate1.x = properties.get<float>("Virus2.rx");
  rotate1.y = properties.get<float>("Virus2.ry");
  rotate1.z = properties.get<float>("Virus2.rz");

  angle1 = properties.get<float>("Virus2.angle");

  Virus2 = glm::translate(translate1)*glm::rotate(angle1, rotate1);

  translate1.x = properties.get<float>("Virus3.tx");
  translate1.y = properties.get<float>("Virus3.ty");
  translate1.z = properties.get<float>("Virus3.tz");

  rotate1.x = properties.get<float>("Virus3.rx");
  rotate1.y = properties.get<float>("Virus3.ry");
  rotate1.z = properties.get<float>("Virus3.rz");

  angle1 = properties.get<float>("Virus3.angle");

  Virus3 = glm::translate(translate1)*glm::rotate(angle1, rotate1);

  vessel_model->shininess = properties.get<float>("Virus1.shininess");
  vessel_model->ambient_factor = properties.get<float>("Virus1.ambient");

  vessel_model->diffuse_color.r = properties.get<float>("Virus1.diffr");
  vessel_model->diffuse_color.g = properties.get<float>("Virus1.diffg");
  vessel_model->diffuse_color.b = properties.get<float>("Virus1.diffb");

  vessel_model->specular_color.r = properties.get<float>("Virus1.specr");
  vessel_model->specular_color.g = properties.get<float>("Virus1.specg");
  vessel_model->specular_color.b = properties.get<float>("Virus1.specb");
}

void MyCells::render(int s, double t) {
	vessel_model->M = &M_Vessel;
	vessel_model->render();

	vessel_model->M = &Virus2;
	vessel_model->render();

	vessel_model->M = &Virus3;
	vessel_model->render();
}