#include "ShaderBuilder.h"
#include <iostream>
#include <fstream>

unsigned long getFileLength(std::ifstream& file)
{
	if (!file.good()) return 0;

	file.seekg(0, std::ios::end);
	unsigned long len = (unsigned long)file.tellg();
	file.seekg(std::ios::beg);

	return len;
}

GLchar* loadshader(std::string filename)
{
	std::ifstream file;
	file.open(filename.c_str(), std::ios::in);
	if (!file)
	{
		std::cout << "FILE " << filename.c_str() << " NOT FOUND"<< std::endl;
		return NULL;
	}
	unsigned long len = getFileLength(file);

	if (len == 0) return NULL; // Error: Empty File

	GLchar* ShaderSource = 0;

	ShaderSource = new char[len + 1];

	if (ShaderSource == 0) return NULL;

	ShaderSource[len] = 0;

	unsigned int i = 0;
	while (file.good())
	{
		ShaderSource[i] = file.get();
		if (!file.eof())
			i++;
	}

	ShaderSource[i] = 0;

	file.close();

	return ShaderSource;
}

void unloadshader(GLchar** ShaderSource)
{
	if (*ShaderSource != 0)delete[] * ShaderSource;
	*ShaderSource = 0;
}

void attachShader(GLuint &program, const char* fileName, GLuint shaderType)
{
	GLuint shader;
	const GLchar *shaderSource[] = { loadshader(fileName) };

	// Create and compile the vertex shader
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, shaderSource, NULL);
	glCompileShader(shader);
	GLint status;

	// Check for compilation errors
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		std::cout << "\n\n" << fileName << std::endl; // show which shader has the error

		fprintf(stderr, "Compilation error in shader vertex_shader: \n%s", strInfoLog);
		delete[] strInfoLog;
	}

	glAttachShader(program, shader);

	glDeleteShader(shader);
	unloadshader((GLchar**)shaderSource);
}

GLuint generateProgram(const char* vertexFilename, const char* fragmentFilename)
{
	GLuint program = glCreateProgram();
	attachShader(program, vertexFilename, GL_VERTEX_SHADER);
	attachShader(program, fragmentFilename, GL_FRAGMENT_SHADER);
	glLinkProgram(program);
	return program;
}

GLuint generateProgram(const char* vertexFilename, const char* geometryFilename, const char* fragmentFilename)
{
	GLuint program = glCreateProgram();
	attachShader(program, vertexFilename, GL_VERTEX_SHADER);
	attachShader(program, geometryFilename, GL_GEOMETRY_SHADER);
	attachShader(program, fragmentFilename, GL_FRAGMENT_SHADER);
	glLinkProgram(program);
	return program;
}

GLuint generateProgram(const char* vertexFilename, const char* geometryFilename, const char* tessContFilename, const char* tessEvalFilename, const char* fragmentFilename)
{
	GLuint program = glCreateProgram();
	attachShader(program, vertexFilename, GL_VERTEX_SHADER);
	attachShader(program, geometryFilename, GL_GEOMETRY_SHADER);
	attachShader(program, tessContFilename, GL_TESS_CONTROL_SHADER);
	attachShader(program, tessEvalFilename, GL_TESS_EVALUATION_SHADER);
	attachShader(program, fragmentFilename, GL_FRAGMENT_SHADER);
	glLinkProgram(program);
	return program;
}
