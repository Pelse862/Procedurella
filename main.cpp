#define PI 3.14159265359f
#include "iostream"
//Vector library
#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtx\transform.hpp"
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
	float rad = 0.4f;
	Sphere S(0, 0, 0, rad);
	float altitude = 0.35f;
	//In varables and Locators
	glm::mat4 projection = glm::perspective(60.f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 10000.0f);
	glm::mat4 rot(1.f);  

	GLint locationProjection = glGetUniformLocation(shader.getProgramId(), "projection");
	GLint locationColorGround = glGetUniformLocation(shader.getProgramId(), "colorSand");
	GLint locationColorPeaks = glGetUniformLocation(shader.getProgramId(), "colorGrass");
	GLint locationColorWater = glGetUniformLocation(shader.getProgramId(), "colorWater");
	GLint locationColorMountain = glGetUniformLocation(shader.getProgramId(), "colorMountain");
	GLint locationAltitude = glGetUniformLocation(shader.getProgramId(), "time");
	GLint locationTime = glGetUniformLocation(shader.getProgramId(), "altitude");
	GLint locationRotation = glGetUniformLocation(shader.getProgramId(), "rotation");
	GLint locationOffset = glGetUniformLocation(shader.getProgramId(), "offset");

	//gul, bkå, grön
	float colorS[] = { 1.f, 1.0f, 0.0 };
	float colorP[] = { 0.0, 1.0, 0.0 };
	float colorW[] = { 0.0, 0.0, 1.0 };
	float colorM[] = { 0.86, 0.86, 0.86 };

	float lastTime = glfwGetTime() - 0.001f;
	float dT = 0.0;
	float offset = 0.01f;
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
		rot = glm::rotate(rot, dT * PI  * (1.f/120.f), glm::vec3(0.f, 1.f , 0.f) );

		//send orojection matrix and color
		glUniformMatrix4fv(locationProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(locationRotation, 1, GL_FALSE, glm::value_ptr(rot));

		glUniform3fv(locationColorGround, 1, &colorS[0]);
		glUniform3fv(locationColorPeaks, 1, &colorP[0]);
		glUniform3fv(locationColorWater, 1, &colorW[0]);
		glUniform3fv(locationColorMountain, 1, &colorM[0]);

		glUniform1f(locationAltitude, rad);
		glUniform1f(locationTime, dT);
		glUniform1f(offset, 6);


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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

