#pragma once

#include "Tools.h"

#include <glm\glm.hpp>
#include <vector>

#define FLOAT_ERR 0.00001f
#define PI 3.14159265359f
#define twoPI 2.f * PI
#define halfPI 0.5f * PI

#define defaultZoom 1.f

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 500

const glm::mat4 identity(1.0f);

const glm::vec3 defaultUp(0.f, 1.f, 0.f),
                defaultCam(0.f, 0.2f, 1.f),
                defaultCenter(0.f, 0.f, 0.f);

void nurbsSpline(std::vector<glm::vec2> &input, std::vector<float> &weights, std::vector<glm::vec2> &output, int order, float uInc);
void generateGeometric(std::vector<glm::vec2> &input, std::vector<float> &weights, std::vector<std::vector<glm::vec2>> &output, int order, float u);



