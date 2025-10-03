#include "Streets.h"

Way::Way(long long id, std::vector<long long>& nodeIDs, Nodes *nodesInstance)
	: id(id), nodeIDs(nodeIDs), nodesInstance(nodesInstance)
{
	for (auto nodeID : nodeIDs) {
		glm::vec3 pos = nodesInstance->nodeMap[nodeID]->vertex;
		vertices.push_back(pos);
	}
	for (int i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}

	wayVAO.Bind();

	VBO wayVBO(vertices);
	EBO wayEBO(indices);

	wayVAO.LinkAttrib(wayVBO, 0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)(0));
	wayVAO.Unbind();

	wayVBO.Unbind();
	wayEBO.Unbind();
}

void Way::Draw()
{
	wayVAO.Bind();
	glDrawElements(GL_LINE_STRIP, GLsizei(indices.size()), GL_UNSIGNED_INT, 0);
	wayVAO.Unbind();
}

Streets::Streets(json& map_data, Nodes* nodesIntance)
{
	this->nodesInstance = nodesInstance;
	int ind = 0;
	if (map_data.contains("elements")) {
		for (const auto& element : map_data["elements"]) {
			if (element["type"] == "way") {
				try {
					long long id = std::stoll(element["id"].dump());
					std::vector<long long> node_arr = element["nodes"];

					Way* currWay = new Way(id, node_arr, nodesIntance);
					ways.push_back(currWay);
					GLsizei vertex_offset = vertices.size();
					vertices.insert(vertices.end(), currWay->vertices.begin(), currWay->vertices.end());
					for (int i = 0; i < currWay->vertices.size() - 1; i++) {
						indices.push_back(vertex_offset + i);
						indices.push_back(vertex_offset + i + 1);
					}
				}
				catch (std::exception& e) {
					std::cerr << e.what();
				}
			}
		}
	}
	streetVAO.Bind();

	VBO streetVBO(vertices);
	EBO streetEBO(indices);

	streetVAO.LinkAttrib(streetVBO, 0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)(0));
	streetVAO.Unbind();

	streetVBO.Unbind();
	streetEBO.Unbind();
}

void Streets::printWayNodes()
{
	for (int i = 0; i < 10; i++) {
		for (auto it : ways[i]->nodeIDs) {
			std::cout << it << " ";
		}
		std::cout << "\n";
		std::cout << "\n";
		std::cout << "\n";
	}
}

void Streets::DrawWays()
{
	for (auto it : ways) {
		it->Draw();
	}
}

void Streets::Draw()
{
	streetVAO.Bind();
	glDrawElements(GL_LINES, GLsizei(indices.size()), GL_UNSIGNED_INT, 0);
	streetVAO.Unbind();
}
