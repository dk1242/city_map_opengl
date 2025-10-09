#include "Application.h"
#include <iostream>
Application::Application(GLFWwindow* window, Camera* camera)
	:m_Window(window), m_Camera(camera)
{
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetKeyCallback(m_Window, keyCallback);
	glfwSetMouseButtonCallback(m_Window, mouseButtonCallback);
	glfwSetCursorPosCallback(m_Window, cursorPositionCallback);
	glfwSetScrollCallback(m_Window, scrollCallback);
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app) {
		app->handleKey(key, scancode, action, mods);
	}
}

void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	//std::cout << "Mouse button event detected." << std::endl;
	auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app) app->handleMouseButton(window, button, action, mods);
}

void Application::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	//std::cout << "cursor event detected." << std::endl;
	auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app) app->handleCursorPosition(window, xpos, ypos);
}

void Application::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	//std::cout << "Mouse scroll event detected." << std::endl;
	auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app) app->handleScroll(window, xoffset, yoffset);
}

void Application::handleKey(int key, int scancode, int action, int mods)
{
	
}

void Application::handleMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			m_IsPanning = true;
			glfwGetCursorPos(window, &m_LastMouseX, &m_LastMouseY);
		}
		else if (action == GLFW_RELEASE) {
			m_IsPanning = false;
		}
	}
}

void Application::handleCursorPosition(GLFWwindow* window, double xpos, double ypos)
{
	
}

void Application::handleScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	
}
