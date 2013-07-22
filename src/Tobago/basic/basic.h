//
//  setup.h
//  Lynx1
//
/*
#pragma once
#ifndef TOVAGO_basic_h
#define TOVAGO_basic_h

#include <cstdlib>
#include "global.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

//from [-1,1]x[-1,1] range to [0, width]x[0, height] range
glm::vec2 screen2pixel(glm::vec2 screenCoords);
//from [0, width]x[0, height] range to [-1,1]x[-1,1] range
glm::vec2 pixel2screen(glm::vec2 pixelCoords);

//from [0,1]x[0,1] range to [0, width]x[0, height] range
glm::vec2 normalized2pixel(glm::vec2 normCoords);
//from [0, width]x[0, height] range to [0,1]x[0,1] range
glm::vec2 pixel2normalized(glm::vec2 pixelCoords);

//from [0,1]x[0,1] range to [-1,1]x[-1,1] range
glm::vec2 normalized2screen(glm::vec2 normCoords);
//from [-1,1]x[-1,1] range to [0, 1]x[0, 1] range
glm::vec2 screen2normalized(glm::vec2 screenCoords);

double randValue(float min, float max);

float maxBeat(int loIndex, int hiIndex);
bool beatDetect(int loIndex, int hiIndex, float cutOff);

#endif
*/