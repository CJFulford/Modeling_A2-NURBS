#pragma once

#include <glm\glm.hpp>
#include <iostream>
#include <vector>

void printVec(glm::vec2 v);
void printVec(glm::vec3 v);
void printVecArray(glm::vec2 *v, int size);
void printVecArray(glm::vec3 *v, int size);
void printVecVector(std::vector<glm::vec2> v);
void printVecVector(std::vector<glm::vec3> v);
void printFloatVector(std::vector<float> v);
glm::vec3 rodriguesRotate(glm::vec3 vector, glm::vec3 axis, float angle);