#pragma once

#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "string"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

class createShader
{

public:
	createShader() {
		shaderId = 0;
		program = glCreateProgram();
	}

	std::string readFile(const char *filepath);
	void use();
	void creteVertexShader(const char *vertexFilePath);
	GLuint getProgramId() const { return shaderId; }
	void creteFragmentShader(const char *fragmentFilePath);
	void detatch();
	~createShader() = default;
private:
	GLint isCompiled = 0;
	GLint isLinked = 0;
	char str[4000];
	GLuint shaderId;
	GLuint program;
	GLuint fragmentShader;
	GLuint vertexShader;
};

