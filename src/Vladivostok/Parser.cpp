#include "Parser.h"

CSParser::CSParser(std::string filename) {
	this->filename = filename;
}

void CSParser::parse() {
  try {
    read_info(filename, pr);
  }
  catch (boost::property_tree::info_parser_error e) {
	  TOBAGO::log.write(ERROR) << "Could not read config file " << filename;
	  return;
  }

  data.width		 = pr.get<int>("Setup.Screen.width");
  data.height		 = pr.get<int>("Setup.Screen.height");
  data.windowTitle	 = pr.get<std::string>("Setup.Screen.title");

  data.oglMajor		 = pr.get<int>("Setup.OpenGL.Major");
  data.oglMinor		 = pr.get<int>("Setup.OpenGL.Minor");

  data.FOV			 = pr.get<float>("View.Perspective.FOV");
  data.zNear		 = pr.get<float>("View.Perspective.zNear");
  data.zFar			 = pr.get<float>("View.Perspective.zFar");

  data.CamPos		 = toVec3(pr.get<std::string>("View.Camera.Position"));
  data.CamDir		 = glm::normalize(toVec3(pr.get<std::string>("View.Camera.Direction")));
  data.CamUp		 = glm::normalize(toVec3(pr.get<std::string>("View.Camera.Up")));

  data.FocalLength	 = pr.get<float>("View.Lens.FocalLength");
  data.FocusDistance = pr.get<float>("View.Lens.FocusDistance");
  data.FStop		 = pr.get<float>("View.Lens.FStop");

  data.EyeSep		 = pr.get<float>("View.Rig.EyeSep");

  data.nlights		 = pr.get<int>("Lights.number");
  data.lights = vector<Light::lamp>(data.nlights);

  std::string name = "Lights.Light0";

  for(int i=0; i<data.nlights; i++) {
	  name[12]++;
	  if(pr.get<std::string>(name + ".type") == "DIRECTIONAL") {
		  data.lights[i].type = Light::DIRECTIONAL;
		  data.lights[i].Attenuation = toVec3(pr.get<std::string>(name + ".attenuation"));
		  data.lights[i].Direction   = glm::normalize(toVec3(pr.get<std::string>(name + ".direction")));
		  data.lights[i].Colour	     = toVec3(pr.get<std::string>(name + ".colour"));
	  } else if(pr.get<std::string>(name + ".type") == "POINT") {
		  data.lights[i].type = Light::POINT;
		  data.lights[i].Attenuation  = toVec3(pr.get<std::string>(name + ".attenuation"));
		  data.lights[i].Position     = toVec3(pr.get<std::string>(name + ".position"));
		  data.lights[i].Colour		  = toVec3(pr.get<std::string>(name + ".colour"));
	  } else if(pr.get<std::string>(name + ".type") == "SPOTLIGHT") {
		  data.lights[i].type = Light::SPOTLIGHT;

  		  data.lights[i].Attenuation = toVec3(pr.get<std::string>(name + ".attenuation"));
		  data.lights[i].Direction   = glm::normalize(toVec3(pr.get<std::string>(name + ".direction")));
		  data.lights[i].Colour	     = toVec3(pr.get<std::string>(name + ".colour"));
		  data.lights[i].Position    = toVec3(pr.get<std::string>(name + ".position"));
		  data.lights[i].OuterCutoff = pr.get<float>(name + ".outerCutoff");
		  data.lights[i].InnerCutoff = pr.get<float>(name + ".innerCutoff");
		  data.lights[i].Exponent	 = pr.get<float>(name + ".exponent");
	  }
  }
}

glm::vec3 CSParser::toVec3(const std::string& s) {
	glm::vec3 result;

	std::stringstream ss(s);
	std::string item;

	std::getline(ss, item, ',');
	result[0] = boost::lexical_cast<float>(item);
	std::getline(ss, item, ',');
	result[1] = boost::lexical_cast<float>(item);
	std::getline(ss, item, ',');
	result[2] = boost::lexical_cast<float>(item);

	return result;
}

float CSParser::getf(const std::string& s) {
	return pr.get<float>(s);
}

void CSParser::passToLight(Light *l) {
	l->numLights = data.nlights;
	for(int i=0; i<std::min(l->MAX_LIGHTS, data.nlights); i++) {
		l->lights[i].type = data.lights[i].type;
		l->lights[i].Position = data.lights[i].Position;
		l->lights[i].Attenuation = data.lights[i].Attenuation;
		l->lights[i].Direction = data.lights[i].Direction;
		l->lights[i].Colour = data.lights[i].Colour;
		l->lights[i].OuterCutoff = data.lights[i].OuterCutoff;
		l->lights[i].InnerCutoff = data.lights[i].InnerCutoff;
		l->lights[i].Exponent = data.lights[i].Exponent;
	}
}