#ifndef NODES_CLASS_H
#define NODES_CLASS_H

#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "json.hpp"
#include "VAO.h"

using json = nlohmann::json;

class Node {
public:
	long long id;
	glm::vec3 vertex;

	Node(long long &id, glm::vec3 &pos)
		:id(id), vertex(pos) {
	}
};
class Nodes {
public:
	float min_lon = std::numeric_limits<float>::max(), max_lon = std::numeric_limits<float>::min(),
		min_lat = std::numeric_limits<float>::max(), max_lat = std::numeric_limits<float>::min();
	std::vector<Node*>nodes;
	std::map<long long, Node*>nodeMap;
	VAO mainVAO;

	Nodes(json &map_data);

	std::vector<glm::vec3> getVertices();
	void Draw();
};
#endif