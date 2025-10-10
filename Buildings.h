#ifndef BUILDINGS_CLASS_H
#define BUILDINGS_CLASS_H

#include "EBO.h"
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "json.hpp"
#include "VAO.h"
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderClass.h"

class Building {
public:
	long long id;
	std::vector<long long>nodeIDs;
	std::vector<glm::dvec3>vertices;
	std::vector<GLuint>indices;

	Building(long long id, std::vector<long long>& nodeIDs, std::map<long long, glm::dvec3>& nodeVerticesMap, float &height);
};

class Buildings {
public:
	std::vector<Building*>buildings;
	std::vector<glm::vec3>vertices;
	std::vector<glm::vec3>normals;
	std::vector<GLuint>indices;
	std::map<long long, glm::dvec3>nodeVerticesMap;

	glm::mat4 model = glm::mat4(1.0f);

	Shader* buildingShader;
	VAO buildingVAO;

	Buildings(Shader* shader);

	void init();
	void Draw();
};

#endif
