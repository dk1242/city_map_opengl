#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

class Camera {
	float m_Left, m_Right, m_Bottom, m_Top;
    float m_Rotation = 0.0f;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

public:
    Camera(float left, float right, float bottom, float top);

    void pan(float dx, float dy);
    void zoom(float factor, float world_x, float world_y);
    void rotate();

    glm::mat4 getMVPMatrix() const;

    float getLeft() const;
    float getRight() const;
    float getBottom() const;
    float getTop() const;


};

#endif