#ifndef APPLICATION_CLASS_H
#define APPLICATION_CLASS_H

#include <GLFW/glfw3.h>
#include "Camera.h"

class Application {
    GLFWwindow* m_Window;
    Camera* m_Camera;
    bool m_IsPanning = false;
    double m_LastMouseX = 0.0;
    double m_LastMouseY = 0.0;

public:
    Application(GLFWwindow* window, Camera* camera);

private:
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    void handleMouseButton(GLFWwindow* window, int button, int action, int mods);
    void handleCursorPosition(GLFWwindow* window, double xpos, double ypos);
    void handleScroll(GLFWwindow* window, double xoffset, double yoffset);

};

#endif