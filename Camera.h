#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

class Camera {
public:
    // -438.961 12.6857 126.088
    // 0.274999 - 0.185875 - 0.943306
    glm::vec3 Position = glm::vec3(-438.961f, 1.0f, 126.088f/*0.1f, 5.0f, 0.0f*/);
    glm::vec3 cameraOrientation = glm::vec3(/*0.274999f, - 0.185875f, - 0.943306f*/0.0f, 0.0f, -1.0f);
    glm::vec3 playerOrientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 RightDir = glm::normalize(glm::cross(cameraOrientation, Up));

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    
    float width = 1920.0, height = 1080.0;

    float speed = 0.001f;
    float sensitivity = 100.0f;

    bool firstClick = true;

    Camera();
    
    glm::mat4 cameraMatrix = glm::mat4(1.0f);

    void updateMatrix();
    void Inputs(GLFWwindow* window);
};

#endif