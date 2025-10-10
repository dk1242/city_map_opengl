#define _USE_MATH_DEFINES

#include "WorldMap.h"
#include <cmath>

const double RADIUS_OF_EARTH = 6378137.0; // In meters
const double HALF_CIRCUMFERENCE_OF_EARTH = M_PI * RADIUS_OF_EARTH;

glm::dvec2 projectLatLongFun2(double lat, double lon) {
	double lat_rad = lat * M_PI / 180.0;
	double lon_rad = lon * M_PI / 180.0;

	// Perform the Mercator projection
	double x = lon_rad * RADIUS_OF_EARTH;
	double y = log(tan(M_PI / 4 + lat_rad / 2)) * RADIUS_OF_EARTH;

	return glm::dvec2(x, y);
}

WorldMap::WorldMap(json& map_data, Shader* streetShader, Shader* buildingShader, Camera *camera)
{
	streetsObj = new Streets(streetShader);
	buildingsObj = new Buildings(buildingShader);
	cameraObj = camera;

	if (map_data.contains("elements")) {
		for (const auto& element : map_data["elements"]) {
			if (element["type"] == "node") {
				try {
					long long id = std::stoll(element["id"].dump());
					double lat_double = std::stod(element["lat"].dump());
					double lon_double = std::stod(element["lon"].dump());
					glm::dvec2 projectedCoords = projectLatLongFun2(lat_double, lon_double);

					glm::dvec3 vertexPos = glm::dvec3(projectedCoords.x, projectedCoords.y, 0.0f);
					nodeVerticesMap[id] = vertexPos;
				}
				catch (std::exception& e) {
					std::cerr << e.what();
				}
			}
			else if (element["type"] == "way" && element.contains("tags") && !element["tags"].contains("building")
				&& !element["tags"].contains("railway") && !element["tags"].contains("cables")) {
				try {
					long long id = std::stoll(element["id"].dump());
					std::vector<long long> node_arr = element["nodes"];

					Way* currWay = new Way(id, node_arr, nodeVerticesMap, min_x, max_x, min_y, max_y);
					streetsObj->ways.push_back(currWay);
				}
				catch (std::exception& e) {
					std::cerr << e.what();
				}
			}
			else if (element["type"] == "way" && element.contains("tags") && element["tags"].contains("building")) {
				try {
					long long id = std::stoll(element["id"].dump());
					std::vector<long long> node_arr = element["nodes"];
					float height = 15.0f;
					if (element["tags"].contains("height")) {
						std::string height_str = element["tags"]["height"];
						height = std::stod(height_str);
						//std::cout << height << "\n";
					}	

					Building* currBuilding = new Building(id, node_arr, nodeVerticesMap, height);
					buildingsObj->buildings.push_back(currBuilding);
				}
				catch (std::exception& e) {
					std::cerr << e.what();
				}
			}
		}

		double center_x = (min_x + max_x) / 2.0;
		double center_y = (min_y + max_y) / 2.0;

		for (auto& it : streetsObj->ways) {
			try {
				GLsizei vertex_offset = streetsObj->vertices.size();
				for (int i = 0; i < it->vertices.size(); i++) {
					glm::dvec3 vertexPos = it->vertices[i];
					streetsObj->vertices.push_back(glm::vec3(
						static_cast<float>(vertexPos.x - center_x),
						0.0f,
						-static_cast<float>(vertexPos.y - center_y)));
				}

				for (int i = 0; i < it->vertices.size() - 2; i += 2) {
					streetsObj->indices.push_back(vertex_offset + i + 0);
					streetsObj->indices.push_back(vertex_offset + i + 1);
					streetsObj->indices.push_back(vertex_offset + i + 2);

					streetsObj->indices.push_back(vertex_offset + i + 1);
					streetsObj->indices.push_back(vertex_offset + i + 3);
					streetsObj->indices.push_back(vertex_offset + i + 2);
				}
			}
			catch (std::exception& e) {
				std::cerr << e.what();
			}
		}
		streetsObj->init();


		for (auto& it : buildingsObj->buildings) {
			try {
				GLsizei vertex_offset = buildingsObj->vertices.size();
				for (int i = 0; i < it->vertices.size(); i++) {
					glm::dvec3 vertexPos = it->vertices[i];
					buildingsObj->vertices.push_back(glm::vec3(
						static_cast<float>(vertexPos.x - center_x),
						vertexPos.z,
						-static_cast<float>(vertexPos.y - center_y)));
				}
				int totalVertices = it->nodeIDs.size();
				for (int i = 0; i <totalVertices; i++) {
					GLsizei current_bottom = vertex_offset + i * 2;
					GLsizei current_top = vertex_offset + i * 2 + 1;

					// Indices for the next vertical edge (wrapping around at the end)
					GLsizei next_bottom = vertex_offset + ((i + 1) % totalVertices) * 2;
					GLsizei next_top = vertex_offset + ((i + 1) % totalVertices) * 2 + 1;

					// First triangle of the wall face
					buildingsObj->indices.push_back(current_bottom);
					buildingsObj->indices.push_back(next_bottom);
					buildingsObj->indices.push_back(current_top);

					// Second triangle of the wall face
					buildingsObj->indices.push_back(current_top);
					buildingsObj->indices.push_back(next_bottom);
					buildingsObj->indices.push_back(next_top);

					glm::dvec3 v_current_bottom = buildingsObj->vertices[current_bottom];
					glm::dvec3 v_current_top = buildingsObj->vertices[current_top];

					// Indices for the next vertical edge (wrapping around at the end)
					glm::dvec3 v_next_bottom = buildingsObj->vertices[next_bottom];
					//glm::dvec3 v_next_top = buildingsObj->vertices[next_top];

					glm::dvec3 vector_A = v_current_top - v_current_bottom;
					glm::dvec3 vector_B = v_next_bottom - v_current_bottom;

					glm::dvec3 faceNormal = glm::cross(vector_B, vector_A);

					if (glm::length(faceNormal) < 0.0001f) {
						faceNormal = glm::vec3(0.0f, 0.0f, 1.0f);
					}
					else {
						faceNormal = glm::normalize(faceNormal);
					}

					buildingsObj->normals.push_back(faceNormal); // Normal for current_bottom
					buildingsObj->normals.push_back(faceNormal); // Normal for current_top
					//buildingsObj->normals.push_back(faceNormal); // Normal for next_bottom
					//buildingsObj->normals.push_back(faceNormal); // Normal for next_top
				}
				//std::cout << buildingsObj->normals.size() << " " << buildingsObj->vertices.size() << "\n";
			}
			catch (std::exception& e) {
				std::cerr << e.what();
			}
		}
		buildingsObj->init();
	}

}

void WorldMap::Draw()
{
	streetsObj->streetShader->Activate();
	glUniformMatrix4fv(glGetUniformLocation(streetsObj->streetShader->ID, "mvpMatrix"), 1, GL_FALSE,
			glm::value_ptr(cameraObj->cameraMatrix));
	streetsObj->Draw();

	buildingsObj->buildingShader->Activate();
	glUniformMatrix4fv(glGetUniformLocation(buildingsObj->buildingShader->ID, "mvpMatrix"), 1, GL_FALSE,
		glm::value_ptr(cameraObj->cameraMatrix));
	buildingsObj->Draw();
}
