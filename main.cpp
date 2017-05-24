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
	int WIDTH = 1200, HEIGHT = 900;

	// Open a window and create its OpenGL context
	Window w(WIDTH, HEIGHT, "Procedurellt");
	GLFWwindow* window = w.getWindow();
	if (!setWindow(window))return 0;
	ImGui_ImplGlfwGL3_Init(window, true);

	//shader 1, Planet
	createShader PlanetShader;
	std::string vertPath = "C:/Users/Commando/Documents/Projects/Procedurella/PlanetShaderVert.glsl";
	std::string fragPath = "C:/Users/Commando/Documents/Projects/Procedurella/PlanetShaderFrag.glsl";
	PlanetShader.creteFragmentShader(fragPath.c_str());
	PlanetShader.creteVertexShader(vertPath.c_str());
	PlanetShader.detatch();
	//shader 2, Cloud
	createShader CloudShader;
	vertPath = "C:/Users/Commando/Documents/Projects/Procedurella/CloudShaderVert.glsl";
	fragPath = "C:/Users/Commando/Documents/Projects/Procedurella/CloudShaderFrag.glsl";
	CloudShader.creteFragmentShader(fragPath.c_str());
	CloudShader.creteVertexShader(vertPath.c_str());
	CloudShader.detatch();
	//Generate Panet
	float radPlanet = 0.4f;
	Sphere Planet(0.f, 0.f, 0.f, radPlanet);
	//Generate Panet
	float radCloud = 0.45f;
	Sphere Cloud(0.f, 0.f, 0.f, radCloud);

	
	//In varables and Locators
	glm::mat4 projection = glm::perspective(100.f, float(WIDTH)/ float(HEIGHT), 0.1f, 10000.0f);
	glm::mat4 rotationPlanet(1.f); 
	glm::mat4 rotationCloud(1.f);

	//shader 1
	GLint locationProjection = glGetUniformLocation(PlanetShader.getProgramId(), "projection");
	GLint locationColorGround = glGetUniformLocation(PlanetShader.getProgramId(), "colorSand");
	GLint locationColorPeaks = glGetUniformLocation(PlanetShader.getProgramId(), "colorGrass");
	GLint locationColorWater = glGetUniformLocation(PlanetShader.getProgramId(), "colorWater");
	GLint locationColorMountain = glGetUniformLocation(PlanetShader.getProgramId(), "colorMountain");
	GLint locationDivider = glGetUniformLocation(PlanetShader.getProgramId(), "divider");
	GLint locationRotation = glGetUniformLocation(PlanetShader.getProgramId(), "rotation");
	GLint locationOffset = glGetUniformLocation(PlanetShader.getProgramId(), "offset");
	GLint locationDistanceFromScreen = glGetUniformLocation(PlanetShader.getProgramId(), "DistanceFromScreen");
	GLint locationNoise = glGetUniformLocation(PlanetShader.getProgramId(), "NoiseType");

	//shader 2
	GLint locationProjectionCloud = glGetUniformLocation(CloudShader.getProgramId(), "projection");
	GLint locationDividerCloud = glGetUniformLocation(CloudShader.getProgramId(), "divider");
	GLint locationDistanceFromScreenCloud = glGetUniformLocation(CloudShader.getProgramId(), "DistanceFromScreen");
	GLint locationAlphaCloud = glGetUniformLocation(CloudShader.getProgramId(), "BaseAlpha");
	GLint locationRotationCloud = glGetUniformLocation(CloudShader.getProgramId(), "rotation");
	GLint locationRadiusCloud = glGetUniformLocation(CloudShader.getProgramId(), "Radius");

	//gul, bkå, grön
	float colorS[] = { 1.f, 1.0f, 0.0 };
	float colorP[] = { 0.0, 1.0, 0.0 };
	float colorW[] = { 0.0, 0.0, 1.0 };
	float colorM[] = { 0.86, 0.86, 0.86 };

	float lastTime = glfwGetTime() - 0.001f;
	float dT = 0.0;
	

	//Imgui variables shader 1
	float offset = 0.01f;
	float denomMulti = 10.f;
	float planetRotations[2] = { 0.f };
	float cloudRotations[2] = { 0.f };
	bool selected[2] = { true,false };
	int whoorley = 0;
	float dist = 2;
	//shader 2
	bool useCloud = false;
	bool inheritRotation = true;
	float baseAlpha = 0.01f;
	float cloudMulti = 5;
	float cloudRadius = radCloud;
	//Render Here
	bool GlLine = false;
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
			if (ImGui::BeginMenu("Planet")) {
			ImGui::Text("Planet stuff");
			ImGui::SliderFloat("Offset: ", &offset, 0.f, 100.f);
			ImGui::SliderFloat("Divider: ", &denomMulti, 2.f, 100.f);
			ImGui::SliderFloat("RotateX: ", &planetRotations[0], -0.1f, 0.1f);
			ImGui::SliderFloat("RotateY: ", &planetRotations[1], -0.1f, 0.1f);
			ImGui::SliderFloat("DistanceFromViewport: ", &dist, 0.f, 4.f);
			ImGui::Selectable("1. Classic Perlin noise", &selected[0]);
			ImGui::Selectable("1. Cellular noise", &selected[1]);
			if (selected[0]) { selected[1] = false; }
			else { selected[1] = true; }
			if (ImGui::BeginMenu("Colors"))
			{
				ImGui::Text("Choose yout colors");
				ImGui::Text("set  color lowest level");
				ImGui::ColorEdit3("color 1", colorW);
				ImGui::Text("set color first middle level");
				ImGui::ColorEdit3("color 2", colorS);
				ImGui::Text("set color second middle level");
				ImGui::ColorEdit3("color 3", colorP);
				ImGui::Text("set color last middle level");
				ImGui::ColorEdit3("color 4", colorM);
				ImGui::EndMenu();

			}

			ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Cloud")) {
				ImGui::Text("Cloud stuff");
				ImGui::Checkbox("Use Clouds", &useCloud);
				ImGui::SliderFloat("Thickness: ", &baseAlpha, 0.f, 0.8f);
				ImGui::SliderFloat("Divider: ", &cloudMulti, 0.1f, 100.f);
				ImGui::SliderFloat("RotateX: ", &cloudRotations[0], -0.1f, 0.1f);
				ImGui::SliderFloat("RotateY: ", &cloudRotations[1], -0.1f, 0.1f);
				ImGui::Checkbox("Use PlanetRotation", &inheritRotation);
				ImGui::SliderFloat("Positionscaling", &cloudRadius,-0.3f,1.0f);
				
				ImGui::EndMenu();
			}
			ImGui::Checkbox("Render as Lines", &GlLine);

		}
		if (GlLine)
		{
			
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		//
		// Clear the colorbuffer
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		PlanetShader.use();
		rotationPlanet = glm::rotate(rotationPlanet, planetRotations[0] * dT , glm::vec3(1.0f, 0.0f, 0.0f));
		rotationPlanet = glm::rotate(rotationPlanet, planetRotations[1] * dT, glm::vec3(0.0f, 1.0f, 0.0f));

		//send orojection matrix and color
		glUniformMatrix4fv(locationProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(locationRotation, 1, GL_FALSE, glm::value_ptr(rotationPlanet));
		// colors of the different levels
		glUniform3fv(locationColorGround, 1, &colorS[0]);
		glUniform3fv(locationColorPeaks, 1, &colorP[0]);
		glUniform3fv(locationColorWater, 1, &colorW[0]);
		glUniform3fv(locationColorMountain, 1, &colorM[0]);
		if (selected[0])
		{
			glUniform1i(locationNoise, 0);
		}
		else
		{
			glUniform1i(locationNoise, 1);
		}
		//some noise and placment stuff
		glUniform1f(locationDivider, denomMulti);
		glUniform1f(locationOffset, offset);
		glUniform1f(locationDistanceFromScreen, dist);
		//render sphere
		Planet.render();
		glBindVertexArray(0);
		if (useCloud)
		{
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
			CloudShader.use();
			glUniformMatrix4fv(locationProjectionCloud, 1, GL_FALSE, glm::value_ptr(projection));
			
			glUniform1f(locationDividerCloud, cloudMulti);
			glUniform1f(locationDistanceFromScreenCloud, dist);
			glUniform1f(locationAlphaCloud, baseAlpha); 
			glUniform1f(locationRadiusCloud, cloudRadius);

			if (inheritRotation) {
				glUniformMatrix4fv(locationRotationCloud, 1, GL_FALSE, glm::value_ptr(rotationPlanet));
			}
			else
			{
				rotationCloud = glm::rotate(rotationCloud, cloudRotations[0] * dT, glm::vec3(1.0f, 0.0f, 0.0f));
				rotationCloud = glm::rotate(rotationCloud, cloudRotations[1] * dT, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(locationRotationCloud, 1, GL_FALSE, glm::value_ptr(rotationCloud));
			}
			Cloud.render();
			glBindVertexArray(0);

		}


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
	

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDisable(GL_TEXTURE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

