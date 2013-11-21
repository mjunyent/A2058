#include "basic.h"

glm::vec2 screen2pixel(glm::vec2 screenCoords, GLFWwindow* w) {
	glm::vec2 norm = screen2normalized(screenCoords, w);
	norm.y = 1-norm.y;
	return normalized2pixel(norm, w);
}

glm::vec2 pixel2screen(glm::vec2 pixelCoords, GLFWwindow* w) {
	glm::vec2 norm = pixel2normalized(pixelCoords, w);
	norm.y = 1-norm.y;
	return normalized2screen(norm, w);
}

//from [0,1]x[0,1] range to [0, width]x[0, height] range
glm::vec2 normalized2pixel(glm::vec2 normCoords, GLFWwindow* w) {
	int width, height;
	glfwGetWindowSize(w, &width, &height);
	glm::vec2 sSize(width, height);

	return normCoords*sSize;
}

//from [0, width]x[0, height] range to [0,1]x[0,1] range
glm::vec2 pixel2normalized(glm::vec2 pixelCoords, GLFWwindow* w) {
	int width, height;
	glfwGetWindowSize(w, &width, &height);
	glm::vec2 sSize(width, height);

	return pixelCoords/sSize;
}


//from [0,1]x[0,1] range to [-1,1]x[-1,1] range
glm::vec2 normalized2screen(glm::vec2 normCoords, GLFWwindow* w) {
	normCoords *= 2.0;  //range [0,2]x[0,2]
	return (normCoords - glm::vec2(1,1));
}

//from [-1,1]x[-1,1] range to [0, 1]x[0, 1] range
glm::vec2 screen2normalized(glm::vec2 screenCoords, GLFWwindow* w) {
	screenCoords = (screenCoords + glm::vec2(1,1));
	screenCoords /= 2.0;
	return screenCoords;
}

double randValue(float min, float max) {
	float val = (float)rand(); //0-RAND_MAX
	val /= RAND_MAX; //0, 1
	val *= (max-min); //0, (max-min)
	val += min; //min, max
	return val;
}