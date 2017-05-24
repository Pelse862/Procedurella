#pragma once
#include <string>
#include "GLFW\glfw3.h"
#include <iostream>

class Window
{
public:
	Window(int w, int h, std::string n) :width(w), height(h), name(n){}
	GLFWwindow* getWindow(){
		
		return glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);;
	}
	bool setWindow(GLFWwindow* w) {
		if (w == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		return true;;
	}

	~Window() = default;

private:
	int width;
	int height;
	std::string name;
	
};