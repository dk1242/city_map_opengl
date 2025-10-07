#include "Camera.h"
#include <iostream>

Camera::Camera(float min_x, float max_x, float min_y, float max_y)
	//: m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top) {}
{
	/*float margin = 0.1f;
	m_Left = min_x - margin;
	m_Right = max_x + margin;
	m_Bottom = min_y - margin;
	m_Top = max_y + margin;*/
	float padding = 1.1f; // 10% padding
	float viewport_width = (max_x - min_x) * padding;
	float viewport_height = (max_y - min_y) * padding;

	float m_Aspect = 1920.0 / 720.0f;
	if (m_Aspect > 1.0f) { // Wider than tall
		viewport_width = viewport_width * m_Aspect;
	}
	else { // Taller than wide
		viewport_height = viewport_height / m_Aspect;
	}

	m_Left = - viewport_width / 2.0f;
	m_Right = + viewport_width / 2.0f;
	m_Bottom = - viewport_height / 2.0f;
	m_Top = + viewport_height / 2.0f;

	// std::cout << min_x << " " << max_x << " " << min_y << " " << max_y << "\n";
	// std::cout << m_Left << " " << m_Right << " " << m_Top << " " << m_Bottom << "\n";

	projectionMatrix = glm::ortho(m_Left, m_Right, m_Bottom, m_Top);
	viewMatrix = glm::mat4(1.0f);
}

void Camera::pan(float dx, float dy)
{
	m_Left += dx;
	m_Right += dx;
	m_Bottom += dy;
	m_Top += dy;
	projectionMatrix = glm::ortho(m_Left, m_Right, m_Bottom, m_Top);
}

void Camera::zoom(float factor, float world_x, float world_y)
{
	m_Left = world_x - (world_x - m_Left) / factor;
	m_Right = world_x + (m_Right - world_x) / factor;
	m_Bottom = world_y - (world_y - m_Bottom) / factor;
	m_Top = world_y + (m_Top - world_y) / factor;
	projectionMatrix = glm::ortho(m_Left, m_Right, m_Bottom, m_Top);
}

void Camera::rotate()
{
	m_Rotation += 90.0f;
	if (m_Rotation >= 360.0f) m_Rotation -= 360.0f;
	
	float centerX = (m_Left + m_Right) / 2.0f;
	float centerY = (m_Bottom + m_Top) / 2.0f;
	
	//glm::mat4 cameraTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(-centerX, -centerY, 0.0f)); 
	glm::mat4 cameraRotation = glm::mat4(1.0f);
	// 3. Translate the pivot point back to its original location
	//cameraRotation = glm::translate(cameraRotation, glm::vec3(centerX, centerY, 0.0f));
	// 2. Rotate around the origin (on the Z axis for 2D)
	cameraRotation = glm::rotate(cameraRotation, glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	// 1. Translate the pivot point to the origin
	//cameraRotation = glm::translate(cameraRotation, glm::vec3(-centerX, -centerY, 0.0f));
	
	viewMatrix = cameraRotation ;
}

glm::mat4 Camera::getMVPMatrix() const
{
	return projectionMatrix * viewMatrix;
}

float Camera::getLeft() const
{
	return m_Left;
}

float Camera::getRight() const
{
	return m_Right;
}

float Camera::getBottom() const
{
	return m_Bottom;
}

float Camera::getTop() const
{
	return m_Top;
}