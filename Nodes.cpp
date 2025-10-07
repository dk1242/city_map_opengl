#define _USE_MATH_DEFINES

#include "Nodes.h"
#include <cmath>

const double RADIUS_OF_EARTH = 6378137.0; // In meters
const double HALF_CIRCUMFERENCE_OF_EARTH = M_PI * RADIUS_OF_EARTH;

glm::vec2 projectLatLong(double lat, double lon) {
	double lat_rad = lat * M_PI / 180.0;
	double lon_rad = lon * M_PI / 180.0;

	// Perform the Mercator projection
	double x = lon_rad * RADIUS_OF_EARTH;
	double y = log(tan(M_PI / 4 + lat_rad / 2)) * RADIUS_OF_EARTH;

	return glm::vec2(x, y);
}

Nodes::Nodes(json& map_data)
{
	std::vector<std::pair<long long, glm::vec3>> projected_vertices_pair;
	float min_x = std::numeric_limits<float>::max();
	float max_x = std::numeric_limits<float>::lowest();
	float min_y = std::numeric_limits<float>::max();
	float max_y = std::numeric_limits<float>::lowest();
	if (map_data.contains("elements")) {
		for (const auto& element : map_data["elements"]) {
			if (element["type"] == "node") {
				try {
					long long id = std::stoll(element["id"].dump());
					double lat_double = std::stod(element["lat"].dump());
					double lon_double = std::stod(element["lon"].dump());
					glm::vec2 projectedCoords = projectLatLong(lat_double, lon_double);
					
					min_x = std::min(min_x, projectedCoords.x);
					max_x = std::max(max_x, projectedCoords.x);

					min_y = std::min(min_y, projectedCoords.y);
					max_y = std::max(max_y, projectedCoords.y);

					glm::vec3 vertexPos = glm::vec3(projectedCoords.x, projectedCoords.y, 0.0f);
					
					//std::cout << nodes.back().first << " " << nodes.back().second << "\n";
					projected_vertices_pair.push_back({ id, vertexPos });
				}
				catch (std::exception& e) {
					std::cerr << e.what();
				}
			}
		}
	}
	//std::vector<glm::vec3> normalized_vertices;
	//for (glm::vec3& point : projected_vertices) {
	//	// Transform longitude (x)
	//	//point.x = static_cast<float>((point.x - min_lon) / (max_lon - min_lon) * 1.8 - 0.9);
	//	// Transform latitude (y)
	//	//point.y = static_cast<float>((point.y - min_lat) / (max_lat - min_lat) * 1.8 - 0.9);
	//	//std::cout << "Transformed point: (" << point.x << ", " << point.y << ")" << std::endl;
	//	double normalized_x = (point.x - min_lon) / (max_lon - min_lon) * 2.0 - 1.0;
	//	double normalized_y = (point.y - min_lat) / (max_lat - min_lat) * 2.0 - 1.0;

	//	normalized_vertices.push_back(glm::vec3(normalized_x, normalized_y, 0.0f));
	//}
	float center_x = (min_x + max_x) / 2.0f;
	float center_y = (min_y + max_y) / 2.0f;

	std::vector<glm::vec3> centered_vertices;
	for (const auto& vp : projected_vertices_pair) {
		long long ID = vp.first;
		glm::vec3 vertexPos = vp.second;
		glm::vec3 centeredVertexPos = glm::vec3(vertexPos.x - center_x, vertexPos.y - center_y, vertexPos.z);
		centered_vertices.push_back(centeredVertexPos);

		Node* currNode = new Node(ID, centeredVertexPos);
		nodes.push_back(currNode);
		nodeMap[ID] = currNode;
	}
	mainVAO.Bind();
	VBO mainVBO(centered_vertices);

	mainVAO.LinkAttrib(mainVBO, 0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0);

	mainVAO.Unbind();
	mainVBO.Unbind();

	min_lon = (float)(min_x - center_x);
	max_lon = (float)(max_x - center_x);
	min_lat = (float)(min_y - center_y);
	max_lat = (float)(max_y - center_y);
}

std::vector<glm::vec3> Nodes::getVertices()
{
	std::vector<glm::vec3> vertices;
	for (auto it : nodes) {
		vertices.push_back(it->vertex);
	}
	return vertices;
}

void Nodes::Draw()
{
	mainVAO.Bind();
	glDrawArrays(GL_POINTS, 0, nodes.size());
	mainVAO.Unbind();
}
