#include "Buildings.h"

Building::Building(long long id, std::vector<long long>& nodeIDs, std::map<long long, glm::dvec3>& nodeVerticesMap, float& height)
	: id(id), nodeIDs(nodeIDs)
{
	for (size_t i = 0; i < nodeIDs.size(); i++) {
		glm::dvec3 projectedCoordsBottom = nodeVerticesMap[nodeIDs[i]];
		glm::dvec3 projectedCoordsTop = projectedCoordsBottom;
		projectedCoordsTop.z = height;

		vertices.push_back(projectedCoordsBottom);
		vertices.push_back(projectedCoordsTop);
	}
}

Buildings::Buildings(Shader* shader)
{
	buildingShader = shader;
}

void Buildings::init()
{
	buildingVAO.Bind();

	VBO buildingVBO(vertices); 
	VBO buildingNormalsVBO(normals);
	EBO buildingEBO(indices);

	buildingVAO.LinkAttrib(buildingVBO, 0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)(0));
	buildingVAO.LinkAttrib(buildingNormalsVBO, 1, 3, GL_FLOAT, sizeof(glm::vec3), (void*)(0));
	buildingVAO.Unbind();

	buildingVBO.Unbind();
	buildingNormalsVBO.Unbind();
	buildingEBO.Unbind();

	buildingShader->Activate();
	glUniformMatrix4fv(glGetUniformLocation(buildingShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void Buildings::Draw()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	buildingVAO.Bind();
	glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_INT, 0);
	buildingVAO.Unbind();
}
