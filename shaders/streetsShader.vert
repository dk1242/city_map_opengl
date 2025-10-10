#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 mvpMatrix;

out vec3 vNormal;

void main()
{
    gl_Position = mvpMatrix * vec4(aPos, 1.0);
    // gl_PointSize = 4.0;
    vNormal = vec3(0.0, 1.0, 0.0);
}