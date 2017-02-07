
#include "iostream"
//Vector library
#include "glm.hpp"
#include "gtc\matrix_transform.hpp"

#include "gtc\type_ptr.hpp"
//opengl helps
#include "glew.h"
#include "glfw3.h"
//Help class  to create a windom
#include "Window.h"
//Create shaders
#include "createShader.h"
//generate texture using the SOIL library
#include "Texture.h"
#include "Sphere.h"
// Define the function's prototype

bool glfwCalls();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
bool setWindow(GLFWwindow* w);


int main()
{
	if (!glfwCalls())return 0;
	int WIDTH = 1024, HEIGHT = 768;

	// Open a window and create its OpenGL context
	Window w(WIDTH, HEIGHT, "Procedurellt");
	GLFWwindow* window = w.getWindow();
	if (!setWindow(window))return 0;
	
	//shader 1
	createShader shader;
	std::string vertPath = "C:/Users/Pelle/Documents/Visual Studio 2015/Projects/Procedurella/vertexShader.glsl";
	std::string fragPath = "C:/Users/Pelle/Documents/Visual Studio 2015/Projects/Procedurella/fragmentShader.glsl";
	shader.creteFragmentShader(fragPath.c_str());
	shader.creteVertexShader(vertPath.c_str());
	shader.detatch();
	//Generate texture from path
	Sphere S(0, 0, -3, 0.5f);

	//matrixes and global variables
	glm::mat4 projection = glm::perspective(45.f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	glm::mat4 rot;  
	GLint projLoc = glGetUniformLocation(shader.getProgramId(), "projection");
	GLint locationColor = glGetUniformLocation(shader.getProgramId(), "Color");
	GLint screenWidth = glGetUniformLocation(shader.getProgramId(), "screenWidth");
	GLint time = glGetUniformLocation(shader.getProgramId(), "time");
	GLint rotateLoc = glGetUniformLocation(shader.getProgramId(), "rotate");

	float color[] = { 1.0, 0.1, 0.1 };
	float lastTime = glfwGetTime() - 0.001f;
	float dT = 0.0;

	//Render Here! _/^0_0^\_
	while (!glfwWindowShouldClose(window))
	{
		//get Time for every frame
		if (dT > 1.f / 30.f) {		
			lastTime = glfwGetTime();
		}
		else {
			dT = glfwGetTime() - lastTime;
		}
		glfwPollEvents();
		// Render
		// Clear the colorbuffer
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		rot = glm::rotate(rot, dT*(1.f/30.f), glm::vec3(0.0f, 1.0f, 0.0f));;
		//send orojection matrix and color
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(locationColor, 1, &color[0]);
		glUniform1f(screenWidth, WIDTH);
		glUniform1f(time, dT);
		glUniformMatrix4fv(rotateLoc, 1, GL_FALSE, glm::value_ptr(rot));
		//render sphere
		S.render();

		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;

}

//glfw calls
bool glfwCalls()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}
	glEnable(GL_DEPTH_TEST);
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 
	return true;
}

//Exit on ESC
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


bool setWindow(GLFWwindow* w)
{
	glfwMakeContextCurrent(w); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}

	glfwSetInputMode(w, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback(w, key_callback);
	return true;
}

