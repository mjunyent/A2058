#include "ConfRead.h"

ConfRead::ConfRead(std::string name) {
	filename = name;
	read();
}

void ConfRead::read() {
	ifstream fs(filename);

	if(fs.is_open()) {
	std::string tmp;
	float px, py, pz;

	fs >> MODE;

	fs >> ResX >> ResY;
	fs >> tmp;
	if(tmp == "FULLSCREEN") WindowMode = GLFW_FULLSCREEN;
	else WindowMode= GLFW_WINDOW;

	for(int i=0; i<6; i++) {
		float c;
		fs >> c;
		costs.push_back(c);
	}

	fs >> tmp;
	if(tmp == "ANAGLYPH") mode3d = ANAGLYPH;
	else if(tmp == "SIDEBYSIDE") mode3d = SIDEBYSIDE;
	else if(tmp == "UPNDOWN") mode3d = UPNDOWN;
	else mode3d = NONE;

	fs >> px >> py >> pz;
	position = glm::vec3(px, py, pz);

	fs >> px >> py >> pz;
	direction = glm::vec3(px, py, pz);

	fs >> px >> py >> pz;
	up = glm::vec3(px, py, pz);

	fs >> EyeSep >> CenterDist;

	fs.close();
	}
}