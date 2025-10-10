#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

vec3 lightPos = vec3(100.0, 100.0, 100.0);
void main()
{
    //vec3 lightDir = vec3(0.3, 0.7, 0.5);
    // Ambient lighting
    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * vec3(1.0);

    // Diffuse lighting
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = vec3(0.3, 0.7, 0.5);//normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    vec3 result = (ambient + diffuse) * vec3(1.0f, 0.5f, 0.2f);

    FragColor = vec4(result, 1.0f);
}