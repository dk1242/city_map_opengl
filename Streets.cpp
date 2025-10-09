#define _USE_MATH_DEFINES
#include "Streets.h"
#include <cmath>

const double RADIUS_OF_EARTH = 6378137.0; // In meters
const double HALF_CIRCUMFERENCE_OF_EARTH = M_PI * RADIUS_OF_EARTH;

Way::Way(long long id, std::vector<long long>& nodeIDs, std::map<long long, glm::dvec3> &nodeVerticesMap,
	double &min_x, double& max_x, double& min_y, double& max_y)
	: id(id), nodeIDs(nodeIDs)
{
	double halfWidth = 3.0f;
	min_x = std::min(min_x, (double)nodeVerticesMap[nodeIDs[0]].x);
	max_x = std::max(max_x, double(nodeVerticesMap[nodeIDs[0]].x));

	min_y = std::min(min_y, (double)nodeVerticesMap[nodeIDs[0]].y);
	max_y = std::max(max_y, (double)nodeVerticesMap[nodeIDs[0]].y);
	for (size_t i = 1; i < nodeIDs.size(); i++) {
		glm::dvec3 projectedCoords = nodeVerticesMap[nodeIDs[i]];

		min_x = std::min(min_x, (double)projectedCoords.x);
		max_x = std::max(max_x, double(projectedCoords.x));

		min_y = std::min(min_y, (double)projectedCoords.y);
		max_y = std::max(max_y, (double)projectedCoords.y);

		glm::dvec3 p1 = nodeVerticesMap[nodeIDs[i-1]];
		glm::dvec3 p2 = nodeVerticesMap[nodeIDs[i]];

		glm::dvec3 direction = glm::normalize(p2 - p1);
		glm::dvec3 normal = glm::vec3(-direction.y, direction.x, 0.0f);

		glm::dvec3 left1 = p1 + normal * halfWidth;
		glm::dvec3 right1 = p1 - normal * halfWidth;
		glm::dvec3 left2 = p2 + normal * halfWidth;
		glm::dvec3 right2 = p2 - normal * halfWidth;

		vertices.push_back(left1);
		vertices.push_back(right1);
		vertices.push_back(left2);
		vertices.push_back(right2);
	}
}

glm::dvec2 projectLatLongFun(double lat, double lon) {
	double lat_rad = lat * M_PI / 180.0;
	double lon_rad = lon * M_PI / 180.0;

	// Perform the Mercator projection
	double x = lon_rad * RADIUS_OF_EARTH;
	double y = log(tan(M_PI / 4 + lat_rad / 2)) * RADIUS_OF_EARTH;

	return glm::dvec2(x, y);
}
void Streets::init() {
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
	/*for (auto it : ways) {
		it->Draw();
	}*/
}

void Streets::Draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	streetVAO.Bind();
	glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_INT, 0);
	streetVAO.Unbind();
}
