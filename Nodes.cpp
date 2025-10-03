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
	std::vector<glm::vec3> vertices;
	if (map_data.contains("elements")) {
		for (const auto& element : map_data["elements"]) {
			if (element["type"] == "node") {
				try {
					long long id = std::stoll(element["id"].dump());
					double lat_double = std::stod(element["lat"].dump());
					double lon_double = std::stod(element["lon"].dump());
					glm::vec2 projectedCoords = projectLatLong(lat_double, lon_double);
					min_lat = std::min(min_lat, lat_double);
					max_lat = std::max(max_lat, lat_double);

					min_lon = std::min(min_lon, lon_double);
					max_lon = std::max(max_lon, lon_double);

					glm::vec3 vertexPos = glm::vec3(lon_double, lat_double, 0.0f);
					Node* currNode = new Node(id, vertexPos);
					nodes.push_back(currNode);
					nodeMap[id] = currNode;
					//std::cout << nodes.back().first << " " << nodes.back().second << "\n";
					vertices.push_back(vertexPos);
				}
				catch (std::exception& e) {
					std::cerr << e.what();
				}
			}
		}
	}
	//for (glm::vec3& point : nodes) {
	//	// Transform longitude (x)
	//	point.x = static_cast<float>((point.x - min_lon) / (max_lon - min_lon) * 1.8 - 0.9);
	//	// Transform latitude (y)
	//	point.y = static_cast<float>((point.y - min_lat) / (max_lat - min_lat) * 1.8 - 0.9);
	//	//std::cout << "Transformed point: (" << point.x << ", " << point.y << ")" << std::endl;
	//}
	mainVAO.Bind();
	VBO mainVBO(vertices);

	mainVAO.LinkAttrib(mainVBO, 0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0);

	mainVAO.Unbind();
	mainVBO.Unbind();
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
