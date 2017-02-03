#include "createShader.h"


//read and compile a vertexShader
void createShader::creteVertexShader(const char *vertexFilePath)
{
	std::string vertexSource = readFile(vertexFilePath);
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *sourceVertex = (const GLchar *)vertexSource.c_str();
	glShaderSource(vertexShader, 1, &sourceVertex, 0);
	glCompileShader(vertexShader);
	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {

		glGetShaderInfoLog(vertexShader, sizeof(str), NULL, str);
		fprintf(stderr, "%s: %s\n", "Vertex shader compile error", str);

		glDeleteShader(vertexShader);

		return;
	}


	glAttachShader(program, vertexShader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);

	if (isLinked == GL_FALSE) {

		glGetProgramInfoLog(program, sizeof(str), NULL, str);
		fprintf(stderr, "%s: %s\n", "Program object linking error", str);

		glDeleteProgram(program);
		glDeleteShader(vertexShader);

		return;
	}

}


void createShader::creteFragmentShader(const char *vertexFilePath)
{
	std::string fragmentSource = readFile(vertexFilePath);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar *sourceFragment = (const GLchar *)fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &sourceFragment, 0);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE) {

		glGetShaderInfoLog(fragmentShader, sizeof(str), NULL, str);
		fprintf(stderr, "%s: %s\n", "Fragment shader compile error", str);

		glDeleteShader(fragmentShader);
		return;
	}
	// create program object

	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);

	if (isLinked == GL_FALSE) {

		glGetProgramInfoLog(program, sizeof(str), NULL, str);
		fprintf(stderr, "%s: %s\n", "Program object linking error", str);

		glDeleteProgram(program);
		glDeleteShader(fragmentShader);

		return;
	}


}
void createShader::detatch()
{
	shaderId = program;

	glDetachShader(program, fragmentShader);
	glDetachShader(program, vertexShader);

	glDeleteShader(fragmentShader);
}


std::string createShader::readFile(const char *filePath) {

	std::string sourceCode;
	std::string line = "";
	std::ifstream fileStream(filePath);

	if (fileStream.is_open()) {

		while (!fileStream.eof()) {

			std::getline(fileStream, line);
			sourceCode.append(line + "\n");
		}
	}
	else {
		std::cerr << "Could not open file: " << filePath << std::endl;
		return "";
	}

	return sourceCode;
}