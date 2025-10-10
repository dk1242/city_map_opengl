#version 330 core

in vec3 vNormal;

out vec4 FragColor;

void main()
{
    vec3 lightDir = vec3(0.3, 0.7, 0.5);
    float diff = max(dot(normalize(vNormal), lightDir), 0.0);
    vec3 color = vec3(0.7) * (0.3 + 0.7 * diff);
   // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    FragColor = vec4(color, 1.0f);
}