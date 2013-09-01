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
