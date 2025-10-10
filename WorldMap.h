#ifndef WORLDMAP_CLASS_H
#define WORLDMAP_CLASS_H

#include "Streets.h"
#include "Buildings.h"
#include "Camera.h"

class WorldMap {
public:
	double min_x = std::numeric_limits<double>::max();
	double max_x = std::numeric_limits<double>::lowest();
	double min_y = std::numeric_limits<double>::max();
	double max_y = std::numeric_limits<double>::lowest();

	std::map<long long, glm::dvec3>nodeVerticesMap;

	Camera* cameraObj;
	Streets* streetsObj;
	Buildings* buildingsObj;

	WorldMap(json& map_data, Shader* streetShader, Shader *buildingShader, Camera *camera);

	void Draw();
};

#endif