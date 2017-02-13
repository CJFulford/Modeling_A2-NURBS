#pragma once

#include <glad\glad.h>

GLuint generateProgram(const char* vertexFilename, const char* fragmentFilename);
GLuint generateProgram(const char* vertexFilename, const char* geometryFilename, const char* fragmentFilename);
GLuint generateProgram(const char* vertexFilename, const char* geometryFilename, const char* tessContFilename, const char* tessEvalFilename, const char* fragmentFilename);