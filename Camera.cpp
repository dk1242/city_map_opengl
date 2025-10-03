#include "Camera.h"

void Camera::pan(float dx, float dy)
{
	m_Left += dx;
	m_Right += dx;
	m_Bottom += dy;
	m_Top += dy;
}

void Camera::zoom(float factor, float world_x, float world_y)
{
	m_Left = world_x - (world_x - m_Left) / factor;
	m_Right = world_x + (m_Right - world_x) / factor;
	m_Bottom = world_y - (world_y - m_Bottom) / factor;
	m_Top = world_y + (m_Top - world_y) / factor;
}

glm::mat4 Camera::getProjectionMatrix() const
{
	return glm::ortho(m_Left, m_Right, m_Bottom, m_Top);
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
