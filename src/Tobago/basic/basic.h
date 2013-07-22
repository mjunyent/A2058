#pragma once
#ifndef TOBAGO_BASIC
#define TOBAGO_BASIC

#include <cstdlib>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

//from [-1,1]x[-1,1] range to [0, width]x[0, height] range
glm::vec2 screen2pixel(glm::vec2 screenCoords, GLFWwindow* w);
//from [0, width]x[0, height] range to [-1,1]x[-1,1] range
glm::vec2 pixel2screen(glm::vec2 pixelCoords, GLFWwindow* w);

//from [0,1]x[0,1] range to [0, width]x[0, height] range
glm::vec2 normalized2pixel(glm::vec2 normCoords, GLFWwindow* w);
//from [0, width]x[0, height] range to [0,1]x[0,1] range
glm::vec2 pixel2normalized(glm::vec2 pixelCoords, GLFWwindow* w);

//from [0,1]x[0,1] range to [-1,1]x[-1,1] range
glm::vec2 normalized2screen(glm::vec2 normCoords, GLFWwindow* w);
//from [-1,1]x[-1,1] range to [0, 1]x[0, 1] range
glm::vec2 screen2normalized(glm::vec2 screenCoords, GLFWwindow* w);

double randValue(float min, float max);

#endif
