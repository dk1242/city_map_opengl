#ifndef STREETS_CLASS_H
#define STREETS_CLASS_H

#include "EBO.h"
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "json.hpp"
#include "VAO.h"
#include "ShaderClass.h"

using json = nlohmann::json;

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
};
class Way {
public:
	long long id;
	std::vector<long long>nodeIDs;
	std::vector<glm::dvec3>vertices;
	std::vector<GLuint>indices;

	Way(long long id, std::vector<long long>& nodeIDs, std::map<long long, glm::dvec3> &nodeVerticesMap,
		double& min_x, double& max_x, double& min_y, double& max_y);

	//void Draw();
};

class Streets {
public:
	std::vector<Way*>ways;
	std::vector<glm::vec3>vertices;
	std::vector<glm::vec3>normals; // will add later
	std::vector<GLuint>indices;
	std::map<long long, glm::dvec3>nodeVerticesMap;

	glm::mat4 model = glm::mat4(1.0f);

	VAO streetVAO;
	Shader* streetShader;

	Streets(Shader* shader);

	void init();
	void printWayNodes();
	void DrawWays();
	void Draw();
};
#endif