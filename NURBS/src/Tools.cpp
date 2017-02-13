#include "Tools.h"

void printVec(glm::vec2 v)
{
	std::cout << v.x << "\t" << v.y << std::endl;
}
void printVec(glm::vec3 v)
{
	std::cout << v.x << "\t" << v.y << "\t" << v.z << std::endl;
}
void printVecArray(glm::vec2 *v, int size)
{
	for (int i = 0; i < size; i++)
		printVec(v[i]);
}
void printVecArray(glm::vec3 *v, int size)
{
	for (int i = 0; i < size; i++)
		printVec(v[i]);
}
void printVecVector(std::vector<glm::vec2> v)
{
	for (unsigned int i = 0; i < v.size(); i++)
		printVec(v[i]);
}
void printVecVector(std::vector<glm::vec3> v)
{
	for (unsigned int i = 0; i < v.size(); i++)
		printVec(v[i]);
}
void printFloatVector(std::vector<float> v)
{
    for (unsigned int i = 0; i < v.size(); i++)
        std::printf("%f\n", v[i]);
}

//Rodrigues' rotation formula
glm::vec3 rodriguesRotate(glm::vec3 vector, glm::vec3 axis, float angle)
{
	return glm::vec3((vector * cos(angle)) +
		(glm::cross(axis, vector) * sin(angle)) +
		(axis * glm::dot(axis, vector) * (1.f - cos(angle))));
}

