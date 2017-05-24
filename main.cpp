#define PI 3.14159265359f
#include "iostream"
//Vector library
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\type_ptr.hpp"
//opengl helps
#include "GL\glew.h"
#include "GLFW\glfw3.h"
//Help class  to create a windom
#include "Window.h"
//Create shaders
#include "createShader.h"
//generate texture using the SOIL library
//#include "Texture.h"
//A sphere
#include "Sphere.h"
// Define the function's prototype
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

bool glCalls();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
bool setWindow(GLFWwindow* w);


int main()
{
	if (!glCalls())return 0;
	int WIDTH = 1024, HEIGHT = 768;

	// Open a window and create its OpenGL context
	Window w(WIDTH, HEIGHT, "Procedurellt");
	GLFWwindow* window = w.getWindow();
	if (!setWindow(window))return 0;
	ImGui_ImplGlfwGL3_Init(window, true);

	//shader 1
	createShader PlanetShader;
	std::string vertPath = "C:/Users/Commando/Documents/Projects/Procedurella/vertexShader.glsl";
	std::string fragPath = "C:/Users/Commando/Documents/Projects/Procedurella/fragmentShader.glsl";
	PlanetShader.creteFragmentShader(fragPath.c_str());
	PlanetShader.creteVertexShader(vertPath.c_str());
	PlanetShader.detatch();
	//Generate texture from path
	float rad = 0.4f;
	Sphere Planet(0.f, 0.f, 0, rad);

	float altitude = 0.35f;
	//In varables and Locators
	glm::mat4 projection = glm::perspective(100.f, float(WIDTH)/ float(HEIGHT), 0.1f, 10000.0f);
	glm::mat4 rot(1.f);  

	GLint locationProjection = glGetUniformLocation(PlanetShader.getProgramId(), "projection");
	GLint locationColorGround = glGetUniformLocation(PlanetShader.getProgramId(), "colorSand");
	GLint locationColorPeaks = glGetUniformLocation(PlanetShader.getProgramId(), "colorGrass");
	GLint locationColorWater = glGetUniformLocation(PlanetShader.getProgramId(), "colorWater");
	GLint locationColorMountain = glGetUniformLocation(PlanetShader.getProgramId(), "colorMountain");
	GLint locationDivider = glGetUniformLocation(PlanetShader.getProgramId(), "divider");
	GLint locationRotation = glGetUniformLocation(PlanetShader.getProgramId(), "rotation");
	GLint locationOffset = glGetUniformLocation(PlanetShader.getProgramId(), "offset");
	GLint locationDistanceFromScreen = glGetUniformLocation(PlanetShader.getProgramId(), "DistanceFromScreen");

	//gul, bkå, grön
	float colorS[] = { 1.f, 1.0f, 0.0 };
	float colorP[] = { 0.0, 1.0, 0.0 };
	float colorW[] = { 0.0, 0.0, 1.0 };
	float colorM[] = { 0.86, 0.86, 0.86 };

	float lastTime = glfwGetTime() - 0.001f;
	float dT = 0.0;
	

	//Imgui variables
	float offset = 0.01f;
	float denomMulti = 10.f;
	float rotations[2] = { 0.f };
	float dist = 2;
	//Render Here
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
		// GUI
		ImGui_ImplGlfwGL3_NewFrame();
		{
			ImGui::Text("stuff");
			ImGui::SliderFloat("Offset: ", &offset, 0.f, 100.f);
			ImGui::SliderFloat("Divider: ", &denomMulti, 2.f, 100.f);
			ImGui::SliderFloat("RotateX: ", &rotations[0], -0.1f, 0.1f);
			ImGui::SliderFloat("RotateY: ", &rotations[1], -0.1f, 0.1f);
			ImGui::SliderFloat("DistanceFromViewport: ", &dist, 0.f, 4.f);

		}
		//
		// Clear the colorbuffer
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		PlanetShader.use();
		rot = glm::rotate(rot, rotations[0] * dT , glm::vec3(1.0f, 0.0f, 0.0f));
		rot = glm::rotate(rot, rotations[1] * dT, glm::vec3(0.0f, 1.0f, 0.0f));

		//send orojection matrix and color
		glUniformMatrix4fv(locationProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(locationRotation, 1, GL_FALSE, glm::value_ptr(rot));
		// colors of the different levels
		glUniform3fv(locationColorGround, 1, &colorS[0]);
		glUniform3fv(locationColorPeaks, 1, &colorP[0]);
		glUniform3fv(locationColorWater, 1, &colorW[0]);
		glUniform3fv(locationColorMountain, 1, &colorM[0]);

		//some noise and placment stuff
		glUniform1f(locationDivider, denomMulti);
		glUniform1f(locationOffset, offset);
		glUniform1f(locationDistanceFromScreen, dist);

		
		//render sphere
		Planet.render();

		glBindVertexArray(0);
		
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui::Render();

		// Swap the screen buffers and make glcalls
		glfwSwapBuffers(window);
		if (!glCalls())return 0;
	}
	ImGui_ImplGlfwGL3_Shutdown();

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;

}

//glfw calls
bool glCalls()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDisable(GL_TEXTURE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

