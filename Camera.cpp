#include "Camera.h"
#include <iostream>

Camera::Camera()
{
}

void Camera::updateMatrix()
{
	// Initializes matrices since otherwise they will be the null matrix
	view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	//std::cout << Position.x<< " " << Position.y << " " << Position.z << "\n";
	//std::cout << Orientation.x << " " << Orientation.y << " " << Orientation.z << "\n";
	view = glm::lookAt(Position, Position + cameraOrientation, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(45.0f), (float)1920.0f / 1080.0f, 0.1f, 10000.0f);

	// Sets new camera matrix
	cameraMatrix = projection * view;
}

void Camera::Inputs(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * playerOrientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(playerOrientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -playerOrientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(playerOrientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 2.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.1f;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / static_cast<double>(2)), (height / 2.0));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees
		float rotX = sensitivity * (float)(mouseY - (height / 2.0)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2.0)) / width;
		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newCameraOrientation = glm::rotate(cameraOrientation, glm::radians(-rotX), glm::normalize(glm::cross(cameraOrientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newCameraOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			cameraOrientation = newCameraOrientation;
		}

		// Rotates the Orientation left and right
		//Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
		//Orientation = glm::normalize(Orientation);
		playerOrientation = glm::rotate(playerOrientation, glm::radians(static_cast<float>(-rotY)), Up);
		cameraOrientation = glm::rotate(cameraOrientation, glm::radians(static_cast<float>(-rotY)), Up);

		playerOrientation = glm::normalize(playerOrientation);
		cameraOrientation = glm::normalize(cameraOrientation);
		/*RightDir = glm::normalize(glm::cross(Orientation, Up));
		Up = glm::normalize(glm::cross(RightDir, Orientation));*/

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}
