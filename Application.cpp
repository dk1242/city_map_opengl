#include "Application.h"
#include <iostream>
Application::Application(GLFWwindow* window, Camera* camera)
	:m_Window(window), m_Camera(camera)
{
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetMouseButtonCallback(m_Window, mouseButtonCallback);
	glfwSetCursorPosCallback(m_Window, cursorPositionCallback);
	glfwSetScrollCallback(m_Window, scrollCallback);
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
	if (m_IsPanning) {
		double dx = xpos - m_LastMouseX;
		double dy = ypos - m_LastMouseY;

		int window_width, window_height;
		glfwGetWindowSize(window, &window_width, &window_height);

		float world_width = m_Camera->getRight() - m_Camera->getLeft();
		float world_height = m_Camera->getTop() - m_Camera->getBottom();

		float world_dx = static_cast<float>(dx * world_width / window_width);
		float world_dy = static_cast<float>(dy * world_height / window_height);

		m_Camera->pan(-world_dx, world_dy); // Note: Mouse Y is inverted in GLFW

		m_LastMouseX = xpos;
		m_LastMouseY = ypos;
	}
}

void Application::handleScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	double mouse_x, mouse_y;
	glfwGetCursorPos(window, &mouse_x, &mouse_y);
	int window_width, window_height;
	glfwGetWindowSize(window, &window_width, &window_height);

	float world_x = m_Camera->getLeft() + static_cast<float>(mouse_x / window_width) * (m_Camera->getRight() - m_Camera->getLeft());
	float world_y = m_Camera->getBottom() + (1.0f - static_cast<float>(mouse_y / window_height)) * (m_Camera->getTop() - m_Camera->getBottom());

	float zoom_factor = 1.1f;
	if (yoffset < 0) { // Zoom out
		zoom_factor = 1.0f / zoom_factor;
	}
	m_Camera->zoom(zoom_factor, world_x, world_y);
}
