#ifndef STREETS_CLASS_H
#define STREETS_CLASS_H

#include "Nodes.h"
#include "EBO.h"

class Way {
public:
	long long id;
	std::vector<long long>nodeIDs;
	std::vector<glm::vec3>vertices;
	std::vector<GLuint>indices;
	Nodes* nodesInstance;

	VAO wayVAO;
	
	Way(long long id, std::vector<long long>& nodeIDs, Nodes* nodesInstance);

	void Draw();
};

class Streets {
public:
	std::vector<Way*>ways;
	Nodes* nodesInstance;
	std::vector<glm::vec3>vertices;
	std::vector<GLuint>indices;

	VAO streetVAO;

	Streets(json& map_data, Nodes* nodesIntance);
	
	void printWayNodes();
	void DrawWays();
	void Draw();
};
#endif