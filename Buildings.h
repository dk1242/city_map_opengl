#ifndef BUILDINGS_CLASS_H
#define BUILDINGS_CLASS_H

#include "EBO.h"
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "json.hpp"
#include "VAO.h"

class Building {
public:
	long long id;
	std::vector<long long>nodeIDs;
	std::vector<glm::dvec3>vertices;
	std::vector<GLuint>indices;

	Building(long long id, std::vector<long long>& nodeIDs, std::map<long long, glm::dvec3>& nodeVerticesMap);
};

class Buildings {
public:
	std::vector<Building*>buildings;
	std::vector<glm::vec3>vertices;
	std::vector<GLuint>indices;
	std::map<long long, glm::dvec3>nodeVerticesMap;

	VAO buildingVAO;

	void init();
	void Draw();
};

#endif
