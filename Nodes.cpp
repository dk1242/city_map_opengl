#define _USE_MATH_DEFINES

#include "Nodes.h"
#include <cmath>

const double RADIUS_OF_EARTH = 6378137.0; // In meters
const double HALF_CIRCUMFERENCE_OF_EARTH = M_PI * RADIUS_OF_EARTH;

glm::dvec2 projectLatLong(double lat, double lon) {
	double lat_rad = lat * M_PI / 180.0;
	double lon_rad = lon * M_PI / 180.0;

	// Perform the Mercator projection
	double x = lon_rad * RADIUS_OF_EARTH;
	double y = log(tan(M_PI / 4 + lat_rad / 2)) * RADIUS_OF_EARTH;

	return glm::dvec2(x, y);
}

Nodes::Nodes(json& map_data)
{
	std::vector<std::pair<long long, glm::dvec3>> projected_vertices_pair;
	double min_x = std::numeric_limits<double>::max();
	double max_x = std::numeric_limits<double>::lowest();
	double min_y = std::numeric_limits<double>::max();
	double max_y = std::numeric_limits<double>::lowest();
	if (map_data.contains("elements")) {
		for (const auto& element : map_data["elements"]) {
			if (element["type"] == "node") {
				try {
					long long id = std::stoll(element["id"].dump());
					double lat_double = std::stod(element["lat"].dump());
					double lon_double = std::stod(element["lon"].dump());
					glm::dvec2 projectedCoords = projectLatLong(lat_double, lon_double);
					
					min_x = std::min(min_x, (double)projectedCoords.x);
					max_x = std::max(max_x, double(projectedCoords.x));

					min_y = std::min(min_y, (double)projectedCoords.y);
					max_y = std::max(max_y, (double)projectedCoords.y);

					glm::dvec3 vertexPos = glm::dvec3(projectedCoords.x, projectedCoords.y, 0.0f);
					
					//std::cout << nodes.back().first << " " << nodes.back().second << "\n";
					projected_vertices_pair.push_back({ id, vertexPos });
				}
				catch (std::exception& e) {
					std::cerr << e.what();
				}
			}
		}
	}
	double center_x = (min_x + max_x) / 2.0;
	double center_y = (min_y + max_y) / 2.0;
	// std::cout << "Calculated min_x: " << min_x << ", max_x: " << max_x << std::endl;
	// std::cout << "Calculated min_y: " << min_y << ", max_y: " << max_y << std::endl;
	// std::cout << "Calculated center_x: " << center_x << ", center_y: " << center_y << std::endl;
	double max_extent = std::max(max_x - min_x, max_y - min_y);
	double scale_factor = 1000.0 / max_extent;
	glm::dvec2 org = projected_vertices_pair[0].second;
	std::vector<glm::vec3> centered_vertices;
	int j = 0;
	for (const auto& vp : projected_vertices_pair) {
		long long ID = vp.first;
		glm::dvec3 vertexPos = vp.second;
		glm::vec3 centeredVertexPos = glm::vec3(
			static_cast<float>(vertexPos.x - org.x),
			0.0f,
			static_cast<float>(vertexPos.y - org.y)/* * scale_factor*/);
		
		centered_vertices.push_back(centeredVertexPos);

		Node* currNode = new Node(ID, centeredVertexPos);
		nodes.push_back(currNode);
		nodeMap[ID] = currNode;
		// if (j++ < 10) {
		// 	std::cout << "Centered Vertex: " << centeredVertexPos.x << ", " << centeredVertexPos.y << ", " << centeredVertexPos.z << std::endl;
		// }
	}
	mainVAO.Bind();
	VBO mainVBO(centered_vertices);

	mainVAO.LinkAttrib(mainVBO, 0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0);

	mainVAO.Unbind();
	mainVBO.Unbind();

	//min_lon = (float)(min_x - center_x);
	//max_lon = (float)(max_x - center_x);
	//min_lat = (float)(min_y - center_y);
	//max_lat = (float)(max_y - center_y);
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
